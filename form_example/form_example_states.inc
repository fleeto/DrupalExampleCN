<?php
// $Id$

/**
 * @file
 * An example of how to use the new #states Form API element, allowing
 * dynamic form behavior with very simple setup.
 */

/**
 * This form shows off the #states system by dynamically showing parts of the
 * form based on the state of other parts.
 *
 * The basic idea is that you add a #states property to the element which is
 * to be changed based on some action elsewhere on the form. The #states
 * property lists a change which is to be made, and under what conditions
 * that change should be made.
 *
 * For example, in the 'tests_taken' form element below we have:
 * @code
 * '#states' => array(
 *   'visible' => array(
 *     ':input[name=student_type]' => array('value' => t('High School')),
 *   ),
 * ),
 * @endcode
 * Meaning that the element is to be made visible when the condition is met.
 * The condition is a combination of a jQuery selector (which selects the
 * element we want to test) and a condition for that element. In this case,
 * the condition is whether the value of the 'student_type' element is
 * 'High School'. If it is, this element will be visible.
 *
 * So the syntax is:
 * @code
 * '#states' => array(
 *   'action_to_take_on_this_form_element' => array(
 *     'jquery_selector_for_another_element' => array('condition_type' => value),
 *   ),
 * ),
 * @endcode
 *
 * If you need an action to take place only when two different conditions are
 * true, then you add both of those conditions to the action. See the
 * 'country_writein' element below for an example.
 *
 * Note that the easiest way to select a textfield, checkbox, or select is
 * with the ':input' jquery shortcut, which selects any any of those.
 * @see http://api.jquery.com/input-selector/
 *
 * There are examples below of changing or hiding an element when a checkbox
 * is checked, when a textarea is filled, when a select has a given value.
 *
 *
 * See also @link http://api.drupal.org/api/drupal/developer--topics--forms_api_reference.html/7#states Form API Reference @endlink
 *
 * Outstanding issues:
 *  - Can't hide a fieldset
 *    @see http://drupal.org/node/667944.
 *    @see http://drupal.org/node/767212.
 *  - Can't use radios instead of select.
 *    @see http://drupal.org/node/767268.
 *  - Setting optional/required doesn't actually make an item optional/required.
 *    Instead it just changes CSS around the item.
 *    @see http://drupal.org/node/767738.
 */
function form_example_states_form($form, &$form_state) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Your name'),
    '#required' => FALSE,

    // This field is normally not required, but if the 'make required' checkbox
    // is checked, it will be changed to required.
    '#states' => array(
      'required' => array(  // Action to take.
        ':input[name=optional_checkbox]' => array('checked' => TRUE)
      ),
    ),
  );
  $form['optional_checkbox'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make name (above) required'),
  );

  $form['student_type'] = array(
    '#type' => 'select',
    '#options' => drupal_map_assoc(array(t('High School'), t('Undergraduate'), t('Graduate'))),
    '#title' => t('What type of student are you?')
  );

  // High school information.
  $form['tests_taken'] = array(
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc(array(t('SAT'), t('ACT'))),
    '#title' => t('What standardized tests did you take?'),
    // This #states rule says that this checkboxes array will be visible only
    // when $form['student_type'] is set to t('High School').
    // It uses the jQuery selector :input[name=student_type] to choose the
    // element which triggers the behavior, and then defines the "High School"
    // value as the one that triggers visibility.
    '#states' => array(
      'visible' => array(   // action to take.
        ':input[name=student_type]' => array('value' => t('High School')),
      ),
    ),
  );
  $form['sat_score'] = array(
    '#type' => 'textfield',
    '#title' => t('Your SAT score:'),
    '#size' => 4,

    // This #states rule limits visibility to when the $form['tests_taken']
    // 'SAT' checkbox is checked.
    '#states' => array(
      'visible' => array(  // action to take.
        ':input[name=tests_taken[SAT]]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['act_score'] = array(
    '#type' => 'textfield',
    '#title' => t('Your ACT score:'),
    '#size' => 4,

    // Set this element visible if the ACT checkbox above is
    // checked.
    '#states' => array(
      'visible' => array(  // action to take.
        ':input[name=tests_taken[ACT]]' => array('checked' => TRUE),
      ),
    ),
  );

  // Undergrad information.
  $form['school_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Your college or university:'),

    // Set this element visible if the student_type is Undergraduate, above.
    '#states' => array(
      'visible' => array(  // action to take.
        ':input[name=student_type]' => array('value' => t('Undergraduate')),
      ),
    ),
  );
  $form['school_country'] = array(
    '#type' => 'select',
    '#options' => drupal_map_assoc(array(t('UK'), t('Other'))),
    '#title' => t('In what country is your college or university located?'),

    // Only set visible if the student type is set to Undergraduate above.
    '#states' => array(
      'visible' => array(   // action to take.
        ':input[name=student_type]' => array('value' => t('Undergraduate')),
      ),
    ),

  );
  $form['country_writein'] = array(
    '#type' => 'textfield',
    '#size' => 20,
    '#title' => t('Please enter the name of the country where your college or university is located.'),

    // Here we use two conditions, which is an AND. We want to show this
    // element *only* if the school_country select is 'Other' AND the
    // student_type is 'Undergraduate'. Otherwise, the field might be shown
    // inappropriately if the user has been experimenting while set to
    // "Undergraduate" and then the select changes.
    '#states' => array(
      'visible' => array(  // Action to take: Make visible.
        // Make visible if *both* conditions are true.
        ':input[name=school_country]' => array('value' => t('Other')),
        ':input[name=student_type]' => array('value' => t('Undergraduate')),
      ),
    ),
  );

  // Graduate student information.
  $form['more_info'] = array(
    '#type' => 'textarea',
    '#title' => t('Please describe your graduate studies'),
    // Make this form visible only if the student_type select is 'Graduate'.
    '#states' => array(
      'visible' => array(   // Action to take.
        ':input[name=student_type]' => array('value' => t('Graduate')),
      ),
    ),
  );

  $form['info_provide'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check here if you have provided information above'),
    '#disabled' => TRUE,
    '#states' => array(
      // Mark this checkbox checked if the "more_info" textarea has something
      // in it, if it's 'filled'.
      'checked' => array(      // Action to take: check the checkbox.
        ':input[name=more_info]' => array('filled' => TRUE),
      ),
      // Only show this if we're working on Graduate.
      'visible' => array(      // Action to take.
        ':input[name=student_type]' => array('value' => t('Graduate')),
      ),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit your information'),
  );

  return $form;
}


function form_example_states_form_submit($form, &$form_state) {
  drupal_set_message(t('Submitting values: @values', array('@values' => var_export($form_state['values'], TRUE))));
}
