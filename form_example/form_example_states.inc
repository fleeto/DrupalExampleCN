<?php

/**
 * @file
 * An example of how to use the new #states Form API element, allowing
 * dynamic form behavior with very simple setup.
 */

/**
 * Form状态示例
 *
 * 这个Form根据Form内一部分的状态，来决定其他部分的显示，以此示范了 #states 系统。
 *
 * @ingroup form_example
 *
 * 基本的想法是，为一个Element添加一个#states属性，然后在Form内的某个动作对这个属性进
 * 行修改。#states 属性列出一个可能的变化，并且定义了触发变化的条件。
 *
 *
 * 例如，在 'tests_taken' Form元素中：
 *
 * @code
 * '#states' => array(
 *   'visible' => array(
 *     ':input[name="student_type"]' => array('value' => 'high_school'),
 *   ),
 * ),
 * @endcode
 *
 * 意思是这个元素在符合条件的时候会变成可见。
 * 变成可见的条件是一个jQuery selector和该元素的，以及这一元素对应的条件。
 * 其中的selector用来选择我们将要测试的元素。在本例中，测试条件是'student_type'元素的
 * 返回值是否是'high_school'。如果是，这个元素就变为可见。
 *
 *
 * 语法是：
 *
 * @code
 * '#states' => array(
 *   'action_to_take_on_this_form_element' => array(
 *     'jquery_selector_for_another_element' => array(
 *       'condition_type' => value,
 *     ),
 *   ),
 * ),
 * @endcode
 *
 *
 * 如果你需要一个两个条件同时为True的动作，可以参考下面的'country_writein'示范。
 *
 *
 * 注意最简单的选择textfield, checkbox或者select元素的方法是参考
 * @link http://api.jquery.com/input-selector/ ':input' jquery 缩写 @endlink,
 *
 *
 *
 * 下面还有例子实现了几种方式：当checkbox选中的时候，当填写文本区的时候，以及
 * 当下拉表选项选择了某个值的时候。
 *
 * 参考 drupal_process_states()能获得完整的文档。
 *
 * @see forms_api_reference.html
 */
function form_example_states_form($form, &$form_state) {
  $form['student_type'] = array(
    '#type' => 'radios',
    '#options' => array(
      'high_school'   => t('High School'),
      'undergraduate' => t('Undergraduate'),
      'graduate'      => t('Graduate'),
    ),
    '#title' => t('What type of student are you?'),
  );
  $form['high_school'] = array(
    '#type' => 'fieldset',
    '#title' => t('High School Information'),
    // 这个 #states 规则的意思是：只有 "student_type" 的值设置为 "High School"的时候，
    // "high school"这一fieldset才显示。
    '#states' => array(
      'visible' => array(
        ':input[name="student_type"]' => array('value' => 'high_school'),
      ),
    ),
  );

  // 高中信息
  $form['high_school']['tests_taken'] = array(
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc(array(t('SAT'), t('ACT'))),
    '#title' => t('What standardized tests did you take?'),

    // 这个 #states规则的意义是，只有$form['student_type'] 被设置为
    // t('High School')的时候，这些checkbox才会显示。
    // 这里用到了jQuery选择器 :input[name=student_type]来选择触发条件相关的元素，
    // 然后定义了“High School”这一取值会触发动作。
    '#states' => array(
      // 动作定义
      'visible' => array(
        ':input[name="student_type"]' => array('value' => 'high_school'),
      ),
    ),
  );

  $form['high_school']['sat_score'] = array(
    '#type' => 'textfield',
    '#title' => t('Your SAT score:'),
    '#size' => 4,

    // 这一 #states 规则，在 $form['tests_taken']中'SAT' 被选中的时候，设置其可见。
    '#states' => array(
      // 动作定义
      'visible' => array(
        ':input[name="tests_taken[SAT]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['high_school']['act_score'] = array(
    '#type' => 'textfield',
    '#title' => t('Your ACT score:'),
    '#size' => 4,

    // 当上面的ACT checkbox被选中的时候设置元素可见：
    '#states' => array(
      // 动作定义
      'visible' => array(
        ':input[name="tests_taken[ACT]"]' => array('checked' => TRUE),
      ),
    ),
  );

  // 大学信息。
  $form['undergraduate'] = array(
    '#type' => 'fieldset',
    '#title' => t('Undergraduate Information'),
    // 这个 #states 规则的意思是，只有"student_type"元素的值是“Undergraduate”的时候。
    // 才显示"undergraduate"这一fieldset，
    '#states' => array(
      'visible' => array(
        ':input[name="student_type"]' => array('value' => 'undergraduate'),
      ),
    ),
  );

  $form['undergraduate']['how_many_years'] = array(
    '#type' => 'select',
    '#title' => t('How many years have you completed?'),
    // 这里的选项是整数，不过既然所有的动作都是由客户端DOM执行的，所以我们还是使用字符串。
    '#options' => array(
      1 => t('One'),
      2 => t('Two'),
      3 => t('Three'),
      4 => t('Four'),
      5 => t('Lots'),
    ),
  );

  $form['undergraduate']['comment'] = array(
    '#type' => 'item',
    '#description' => t("Wow, that's a long time."),
    '#states' => array(
      'visible' => array(

        // 注意因为这个取值是来自于DOM的，这里用的是字符串而不是整数。
        ':input[name="how_many_years"]' => array('value' => '5'),
      ),
    ),
  );
  $form['undergraduate']['school_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Your college or university:'),
  );
  $form['undergraduate']['school_country'] = array(
    '#type' => 'select',
    '#options' => drupal_map_assoc(array(t('UK'), t('Other'))),
    '#title' => t('In what country is your college or university located?'),
  );
  $form['undergraduate']['country_writein'] = array(
    '#type' => 'textfield',
    '#size' => 20,
    '#title' => t('Please enter the name of the country where your college or university is located.'),

    // 只在school_country设置为'Other' 的时候显示这个Field
    '#states' => array(
      // 采取的动作：显示
      'visible' => array(
        ':input[name="school_country"]' => array('value' => t('Other')),
      ),
    ),
  );

  $form['undergraduate']['thanks'] = array(
    '#type' => 'item',
    '#description' => t('Thanks for providing both your school and your country.'),
    '#states' => array(
      // 这里的可见性控制，需要两个条件同时为真。
      'visible' => array(
        ':input[name="school_country"]' => array('value' => t('Other')),
        ':input[name="country_writein"]' => array('filled' => TRUE),
      ),
    ),
  );
  $form['undergraduate']['go_away'] = array(
    '#type' => 'submit',
    '#value' => t('Done with form'),
    '#states' => array(
      // 这里的可见性控制，需要两个条件同时为真。
      'visible' => array(
        ':input[name="school_country"]' => array('value' => t('Other')),
        ':input[name="country_writein"]' => array('filled' => TRUE),
      ),
    ),
  );

  // 毕业生信息
  $form['graduate'] = array(
    '#type' => 'fieldset',
    '#title' => t('Graduate School Information'),
    // 这个 #states 规则的意思是，只有在"student_type"元素设置为"Graduate"的情况下，
    // 才显示 'graduate' 这个 fieldset。
    '#states' => array(
      'visible' => array(
        ':input[name="student_type"]' => array('value' => 'graduate'),
      ),
    ),
  );
  $form['graduate']['more_info'] = array(
    '#type' => 'textarea',
    '#title' => t('Please describe your graduate studies'),
  );

  $form['graduate']['info_provide'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check here if you have provided information above'),
    '#disabled' => TRUE,
    '#states' => array(
      // 如果 "more_info" 这一文本域有了内容，那么设置值 "filled" 为真。
      'checked' => array(
        ':input[name="more_info"]' => array('filled' => TRUE),
      ),
    ),
  );

  $form['average'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter your average'),
    // 当一个元素可以有不止一个值能够触发state的时候，
    // 需要把所有值集中到一个更高级别的数组中。
    '#states' => array(
      'visible' => array(
        ':input[name="student_type"]' => array(
          array('value' => 'high_school'),
          array('value' => 'undergraduate'),
        ),
      ),
    ),
  );

  $form['expand_more_info'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check here if you want to add more information.'),
  );
  $form['more_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Additional Information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,

    // 如果checkbox选中，则展开expand_more_info这个fieldset。
    '#states' => array(
      'expanded' => array(
        ':input[name="expand_more_info"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['more_info']['feedback'] = array(
    '#type' => 'textarea',
    '#title' => t('What do you have to say?'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit your information'),
  );

  return $form;
}

/**
 * Submit handler for form_example_states_form().
 */
function form_example_states_form_submit($form, &$form_state) {
  drupal_set_message(t('Submitting values: @values', array('@values' => var_export($form_state['values'], TRUE))));
}
