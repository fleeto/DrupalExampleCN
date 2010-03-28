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
 *
 * The extra argument $no_js_use is here only to allow presentation of this
 * form as if Javascript were not enabled. ajax_example_menu() provides two
 * ways to call this form, one normal ($no_js_use = FALSE) and one simulating
 * Javascript disabled ($no_js_use = TRUE).
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

    // bind an ajax callback to the change event (which is the default for the
    // select form type) of the first dropdown. It will replace the second
    // dropdown when rebuilt.
    '#ajax' => array(
      'callback' => 'ajax_example_dependent_dropdown_degrades_first_callback',
      'wrapper' => 'dropdown-second-replace',
    ),
  );

  // This simply allows us to demonstrate no-javascript use without
  // actually turning off javascript in the browser. Removing the #ajax
  // element turns off AJAX behaviors on that element and as a result
  // ajax.js doesn't get loaded. This is for demonstration purposes only.
  if ($no_js_use) {
    unset($form['dropdown_first_fieldset']['dropdown_first']['#ajax']);
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

/**
 * Submit function for ajax_example_dependent_dropdown_degrades().
 */
function ajax_example_dependent_dropdown_degrades_submit($form, &$form_state) {

  // Now handle the case of the next, previous, and submit buttons.
  // only submit will result in actual submission, all others rebuild.
  switch($form_state['clicked_button']['#id']) {
    case 'edit-submit': // Submit: We're done.
      $form_state['rebuild'] = FALSE;
      drupal_set_message(t('Your values have been submitted. dropdown_first=@first, dropdown_second=@second', array('@first' => $form_state['values']['dropdown_first'], '@second' => $form_state['values']['dropdown_second'])));
      return;
  }

  // edit-next or anything else will cause rebuild of the form and present
  // it again.
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



/**
 * Example of a form with portions dynamically enabled or disabled, but
 * with graceful degradation in the case of no javascript.
 *
 * The idea here is that certain parts of the form don't need to be displayed
 * unless a given option is selected, but then they should be displayed and
 * configured.
 *
 * The third $no_js_use argument is strictly for demonstrating operation
 * without javascript, without making the user/developer turn off javascript.
 */

function ajax_example_dynamic_sections($form, &$form_state, $no_js_use = FALSE) {

  $form['description'] = array(
    '#type' => 'markup',
    '#markup' => '<div>' . t('This example demonstrates a form which dynamically creates various sections based on the configuration in the form.
      It deliberately allows graceful degradation to a non-javascript environment.
      In a javascript environment, the "Choose" button next to the select control
      if displayed; in a non-js environment it is hidden by the module CSS.
      <br/><br/>The basic idea here is that the form is built up based on
      the selection in the question_type_select field, and it is built the same
      whether we are in a javascript/AJAX environment or not.
      <br/><br/>
      Try the <a href="!ajax_link">AJAX version</a> and the <a href="!non_ajax_link">simulated-non-AJAX version</a>.
    ', array('!ajax_link' => url('ajax_example/dynamic_sections'), '!non_ajax_link' => url('ajax_example/dynamic_sections_no_js') )) . '</div>',
  );

  $form['question_type_select'] = array(
    '#type' => 'select',
    '#title' => t('Question style'),
    '#options' => drupal_map_assoc(array(t('Choose question style'), t('Multiple Choice'), t('True/False'), t('Fill-in-the-blanks'))),
    // '#description' => t('Choose whether you want multiple-choice, true/false, or fill-in-the-blanks questions.'),
    '#default_value' => empty($form_state['values']['question_type_select']) ? '' : $form_state['values']['question_type_select'],
    '#ajax' => array(
      'wrapper' => 'questions-fieldset-wrapper',
      'callback' => 'ajax_example_dynamic_sections_select_callback',
    ),
  );
  // The CSS for this module hides this next button if JS is enabled.
  $form['question_type_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Choose'),
    '#attributes' => array('class' => array('next-button')),
  );

  // This simply allows us to demonstrate no-javascript use without
  // actually turning off javascript in the browser. Removing the #ajax
  // element turns off AJAX behaviors on that element and as a result
  // ajax.js doesn't get loaded.

  if ($no_js_use) {
    // Remove the #ajax from the above, so ajax.js won't be loaded.
    unset($form['question_type_select']['#ajax']);
  }

  // This fieldset just serves as a container for the part of the form
  // that gets rebuilt.
  $form['questions_fieldset'] = array(
    '#type' => 'fieldset',

    // These provide the wrapper referred to in #ajax['wrapper'] above.
    '#prefix' => '<div id="questions-fieldset-wrapper">',
    '#suffix' => '</div>',
  );
  if (!empty($form_state['values']['question_type_select'])) {

    $form['questions_fieldset']['question'] = array(
      '#markup' => t('Who was the first president of the U.S.?'),
    );
    $question_type = $form_state['values']['question_type_select'];

    switch ($question_type) {
      case t('Multiple Choice'):
        $form['questions_fieldset']['question'] = array(
          '#type' => 'radios',
          '#title' => t('Who was the first president of the United States'),
          '#default_value' => empty($form_state['values']['questions_fieldset']['question']) ? '' : $form_state['values']['questions_fieldset']['question'],
          '#options' => drupal_map_assoc(array(t('George Bush'), t('Adam McGuire'), t('Abraham Lincoln'), t('George Washington'))),
        );
        break;

      case t('True/False'):
        $form['questions_fieldset']['question'] = array(
          '#type' => 'radios',
          '#title' => t('Was George Washington the first president of the United States?'),
          '#options' => array(t('George Washington') => t("True"), 0 => t("False")),
          '#description' => t('Click "True" if you think George Washington was the first president of the United States.'),
          '#default_value' => empty($form_state['values']['questions_fieldset']['question']) ? '' : $form_state['values']['questions_fieldset']['question'],
        );
        break;

      case t('Fill-in-the-blanks'):
        $form['questions_fieldset']['question'] = array(
          '#type' => 'textfield',
          '#title' => t('Who was the first president of the United States'),
          '#description' => t('Please type the correct answer to the question.'),
          '#default_value' => empty($form_state['values']['questions_fieldset']['question']) ? '' : $form_state['values']['questions_fieldset']['question'],
        );
        break;

    }

    $form['questions_fieldset']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit your answer'),
    );
  }

  return $form;
}

/**
 * Validation function for ajax_example_dynamic_sections().
 *
 * This causes horrible errors until http://drupal.org/node/671184 lands.
 * @todo: Enable this after http://drupal.org/node/671184 lands.
 */
//function ajax_example_dynamic_sections_validate($form, &$form_state) {
//  $answer = $form_state['values']['question'];
//  if ($answer !== t('George Washington')) {
//    form_set_error('question', t('Wrong answer. Try again. (Hint: The right answer is "George Washington".)'));
//  }
//}

/**
 * Submit function for ajax_example_dynamic_sections().
 */
function ajax_example_dynamic_sections_submit($form, &$form_state) {
  // This is only executed when a button is pressed, not when the AJAXified
  // select is changed.
  // Now handle the case of the next, previous, and submit buttons.
  // only submit will result in actual submission, all others rebuild.
  switch($form_state['clicked_button']['#id']) {
    case 'edit-submit': // Submit: We're done.
      $form_state['rebuild'] = FALSE;
      $answer = $form_state['values']['question'];

      // Special handling for the checkbox.
      if ($answer == 1 && $form['questions_fieldset']['question']['#type'] == 'checkbox') {
        $answer = $form['questions_fieldset']['question']['#title'];
      }
      if ($answer === t('George Washington')) {
        drupal_set_message(t('You got the right answer: @answer', array('@answer' => $answer)));
      }
      else {
        drupal_set_message(t('Sorry, your answer (@answer) is wrong', array('@answer' => $answer)));
      }
      $form_state['rebuild'] = FALSE;
      return;
  }

  // edit-next or anything else will cause rebuild of the form and present
  // it again.
  $form_state['rebuild'] = TRUE;
}

/**
 * Callback for the select element.
 *
 * This just selects and returns the questions_fieldset.
 */
function ajax_example_dynamic_sections_select_callback($form, $form_state) {
  return $form['questions_fieldset'];
}