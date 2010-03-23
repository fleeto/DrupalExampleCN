<?php
// $Id$

/**
 * @file
 * Demonstrations of AJAX with graceful degradation.
 *
 */

/**
 * @defgroup ajax_degradation_examples AJAX Degradation Examples
 * @{
 * These examples show AJAX with graceful degradation when Javascript is not
 * available.
 *
 * General documentation is available at
 * @link ajax AJAX Framework documentation @endlink.
 *
 * @}
*/

/**
 * @ingroup ajax_degradation_examples
 * @{
 */


/**
 * A form with a dropdown whose options are dependent on a
 * choice made in a previous dropdown.
 *
 * On changing the first dropdown, the options in the second
 * are updated. Gracefully degrades if no javascript.
 */
function ajax_example_dependent_dropdown_degrades($form, &$form_state, $no_js_use = FALSE) {
  // get the list of options to populate the first dropdown
  $options_first = _ajax_example_get_first_dropdown_options();

  // if we have a value for the first dropdown from $form_state['values'] we use
  // this both as the default value for the first dropdown and also as a
  // parameter to pass to the function that retrieves the options for the
  // second dropdown.
  $selected = isset($form_state['values']['dropdown_first']) ? $form_state['values']['dropdown_first'] : key($options_first);


  $form['dropdown_first_fieldset'] = array(
    '#type' => 'fieldset',
  );
  $form['dropdown_first_fieldset']['dropdown_first'] = array(
    '#type' => 'select',
    '#title' => 'First Dropdown',
    '#options' => $options_first,
    '#default_value' => $selected,
    '#attributes' => array('class' => array('enabled-for-ajax')),
  );

  // This simply allows us to demonstrate no-javascript use without
  // actually turning off javascript in the browser. Removing the #ajax
  // element turns off AJAX behaviors on that element and as a result
  // ajax.js doesn't get loaded.
  // Normally this would be part of the above declaration.

  if (!$no_js_use) {
    // bind an ajax callback to the change event (which is the default for the
    // select form type) of the first dropdown. It will replace the second
    // dropdown when rebuilt
    $form['dropdown_first_fieldset']['dropdown_first']['#ajax'] = array(
      'callback' => 'ajax_example_dependent_dropdown_degrades_first_callback',
      'wrapper' => 'dropdown-second-replace',
    );
  }

  // The CSS for this module hides this next button if JS is enabled.
  $form['dropdown_first_fieldset']['continue_to_second'] = array(
    '#type' => 'submit',
    '#value' => t('Choose'),
    '#attributes' => array('class' => array('next-button')),
  );

  $form['dropdown_second_fieldset'] = array(
    '#type' => 'fieldset',
  );
  $form['dropdown_second_fieldset']['dropdown_second'] = array(
    '#type' => 'select',
    '#title' => 'Second Dropdown',
    '#prefix' => '<div id="dropdown-second-replace">',
    '#suffix' => '</div>',
    '#attributes' => array('class' => array('enabled-for-ajax')),

    // when the form is rebuilt during processing (either AJAX or multistep),
    // the $selected variable will now have the new value and so the options
    // will change.
    '#options' => _ajax_example_get_second_dropdown_options($selected),
    '#default_value' => isset($form_state['values']['dropdown_second']) ? $form_state['values']['dropdown_second'] : '',
  );
//  // The CSS for this module hides this next button if JS is enabled.
//  $form['dropdown_second_fieldset']['continue_to_submit'] = array(
//    '#type' => 'submit',
//    '#value' => t('Continue to submit'),
//    '#attributes' => array('class' => array('next-button')),
//  );

  $form['dropdown_second_fieldset']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('OK'),

    // This class allows attached js file to override the disabled attribute,
    // since it's not necessary in ajax-enabled form.
    '#attributes' => array('class' => array('enabled-for-ajax')),
  );

  // Disable dropdown_second if a selection has not been made on dropdown_first.
  if (empty($form_state['values']['dropdown_first'])) {
    $form['dropdown_second_fieldset']['dropdown_second']['#disabled'] = TRUE;
    $form['dropdown_second_fieldset']['dropdown_second']['#description'] = t('You must make your choice on the first dropdown before changing this second one.');
    $form['dropdown_second_fieldset']['submit']['#disabled'] = TRUE;
  }


  return $form;
}

function ajax_example_dependent_dropdown_degrades_submit($form, &$form_state) {
  // @todo: Note that this gets completely redone when http://drupal.org/node/684846
  // lands. At that point the form submit function will not be run during
  // an AJAX form callback.
  // This first stanza acts much like things will when that lands.
  if (!empty($form_state['input']['ajax_triggering_element'])) {
    $form_state['rebuild'] = TRUE;
    return;
  }

  // Now handle the case of the next, previous, and submit buttons.
  // only submit will result in actual submission, all others rebuild.
  switch($form_state['clicked_button']['#id']) {
    case 'edit-submit': // Submit: We're done.
      $form_state['rebuild'] = FALSE;
      drupal_set_message(t('Your values have been submitted. dropdown_first=@first, dropdown_second=@second', array('@first' => $form_state['values']['dropdown_first'], '@second' => $form_state['values']['dropdown_second'])));
      return;
  }

  // edit-next or anything else will cause rebuild.
  $form_state['rebuild'] = TRUE;
}

/**
 * Selects just the second dropdown to be returned for re-rendering
 *
 * @return renderable array (the second dropdown)
 */
function ajax_example_dependent_dropdown_degrades_first_callback($form, $form_state) {
  return $form['dropdown_second_fieldset']['dropdown_second'];
}
