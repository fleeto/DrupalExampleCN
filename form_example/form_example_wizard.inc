<?php

/**
 * @file
 * Extensible wizard form example.
 */

/**
 * 示例：可扩展的向导Form
 *
 * 这是一个使用了向导风格的多步Form的例子。这里面在需要的时候会提供上一步和下一步按钮，在最
 * 后一步会提供一个结束按钮。
 *
 * 这个例子是一个可以扩展的空壳，其中可以包含更多步骤，甚至可以编程控制。
 * 这个演示包含了三个步骤，每个步骤都有自己的验证函数。
 *
 *
 * 如何扩展这个例子：
 * - 步骤是在_form_example_steps()函数中定义的，可以在这里根据需求加入和修改步骤。
 * - 每一步都可以实现相应的'form'函数，本例中可以参考第一步里面的
 *   'form_example_wizard_personal_info',
 * - 还可以使用普通的Form验证钩子(form_name_validate)加入自定义的验证函数，这个
 *   向导在每一步中都会使用这些验证函数。
 * - 最重要的自定义过程是修改提交处理，并在这里对输入的信息进行处理。本例中只是显示了各个
 *   步骤中搜集的所有信息。
 * @ingroup form_example
 */

/**
 * 返回步骤列表，以及相关联的Form。
 *
 * 这里被单独提出是为了方便理解。这里你可以修改这个函数，来实现自己的向导。
 *
 * @return array
 *   步骤和相关的Form。
 *
 * @ingroup form_example
 */
function _form_example_steps() {
  return array(
    1 => array(
      'form' => 'form_example_wizard_personal_info',
    ),
    2 => array(
      'form' => 'form_example_wizard_location_info',
    ),
    3 => array(
      'form' => 'form_example_wizard_other_info',
    ),
  );
}

/**
 *
 * 向导Form中的首要form builder函数。
 *
 * 这个form应该以drupal_get_form()代码进行调用，他会包含定义中其他的步骤Form。因为他会
 * 处理所有的步骤动作，所以不要修改这个函数。
 *
 *
 * 这个Form有两个定义好的提交处理，用来处理不同的步骤：
 *  - 上一步：处理返回上一步的向导操作。
 *  - 下一步：提交本步骤的Form。
 *
 * 第三个处理按钮是结束，是缺省的form_submit，用来处理信息。
 *
 * 不用修改上一步和下一步按钮，但是必须修改form_example_wizard_submit来执行最后的信息
 * 处理。
 *
 * @ingroup form_example
 */
function form_example_wizard($form, &$form_state) {

  // 为向导初始化一些步骤描述。
  if (empty($form_state['step'])) {
    $form_state['step'] = 1;

    // 这个数组包含了每一步需要调用的函数，其中包含了各个步骤相关的Form元素。
    // 这里还保存了每一步的状态信息。
    $form_state['step_information'] = _form_example_steps();
  }
  $step = &$form_state['step'];
  drupal_set_title(t('Extensible Wizard: Step @step', array('@step' => $step)));

  // 从$form_state['step_information']中获取函数名并调用，得到该步骤需要显示的
  // Form元素。
  $form = $form_state['step_information'][$step]['form']($form, $form_state);

  // 如果合适的话就显示'上一步'按钮。注意 #submit 是被设置为一个特别的提交处理函数，
  // 当我们使用上一步按钮的时候，会使用 #limit_validation_errors 来跳过所有校验
  // 函数产生的信息，并丢弃所有的已输入内容，以免在回退过程中造成干扰。
  if ($step > 1) {
    $form['prev'] = array(
      '#type' => 'submit',
      '#value' => t('Previous'),
      '#name' => 'prev',
      '#submit' => array('form_example_wizard_previous_submit'),
      '#limit_validation_errors' => array(),
    );
  }

  // 如果有后续步骤，则显示“下一步”按钮。
  if ($step < count($form_state['step_information'])) {
    // 每一步都包含下一步按钮。
    $form['next'] = array(
      '#type' => 'submit',
      '#value' => t('Next'),
      '#name' => 'next',
      '#submit' => array('form_example_wizard_next_submit'),
    );
  }
  else {
    // 如果没有更多步骤了，我们会使用向导Form提供的缺省提交过程。你可以称之为结束、提交
    // 或者随便什么东西。当这个按钮被点击时，会触发form_example_wizard_submit。
    $form['finish'] = array(
      '#type' => 'submit',
      '#value' => t('Finish'),
    );
  }

  // 为不同的步骤引入各自的验证函数。
  if (function_exists($form_state['step_information'][$step]['form'] . '_validate')) {
    $form['next']['#validate'] = array($form_state['step_information'][$step]['form'] . '_validate');
  }

  return $form;
}

/**
 * “上一步”按钮的提交处理。
 *
 * 本函数：
 * - 把 $form_state['values'] 保存到别处。
 * - 步骤计数器减一。
 * - 把 $form_state['values']的值替换为前一个状态的值。
 * - 强行重建Form。
 *
 * 这个函数无需改动。
 *
 * @ingroup form_example
 */
function form_example_wizard_previous_submit($form, &$form_state) {
  $current_step = &$form_state['step'];
  $form_state['step_information'][$current_step]['stored_values'] = $form_state['values'];
  if ($current_step > 1) {
    $current_step--;
    $form_state['values'] = $form_state['step_information'][$current_step]['stored_values'];
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * 下一步按钮的提交处理。
 *
 * 本函数：
 * - 把 $form_state['values'] 保存到别处。
 * - 步骤计数器加一。
 * - 如果之前访问过这一个步骤，则用当时存储的值替换当前的 $form_state['values']；否则
 *   给 $form_state['values'] 赋值为 array()。
 * - 强行重建Form。
 *
 * 这个函数无需改动。
 *
 * @ingroup form_example
 */
function form_example_wizard_next_submit($form, &$form_state) {
  $current_step = &$form_state['step'];
  $form_state['step_information'][$current_step]['stored_values'] = $form_state['values'];

  if ($current_step < count($form_state['step_information'])) {
    $current_step++;
    if (!empty($form_state['step_information'][$current_step]['stored_values'])) {
      $form_state['values'] = $form_state['step_information'][$current_step]['stored_values'];
    }
    else {
      $form_state['values'] = array();
    }
    // 强行重建下一个步骤。
    $form_state['rebuild'] = TRUE;
    return;
  }
}

/**
 * 前一段代码是向导Form的基础骨架。不需要对之前的代码进行任何修改。只需要在
 * _form_example_steps() 函数中进行步骤定义。
 *
 * 从这里开始的所有代码是向导的内容，也就是各个步骤的Form。
 *
 * 首先，我们看看对向导Form的步骤定义。
 * @ingroup form_example
 */

/**
 * 返回向导中 'personal info' 页面的Form元素。
 *
 * 这是向导的第一步，提供了两个文本字段：姓和名。
 *
 * @ingroup form_example
 */
function form_example_wizard_personal_info($form, &$form_state) {
  $form = array();
  $form['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name'),
    '#default_value' => !empty($form_state['values']['first_name']) ? $form_state['values']['first_name'] : '',
  );
  $form['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name'),
    '#default_value' => !empty($form_state['values']['last_name']) ? $form_state['values']['last_name'] : '',
  );
  return $form;
}

/**
 * 返回向导中的 'location info' 页面。
 *
 * 这是向导中的第二个步骤。这个步骤提供一个文本字段：城市。
 * 这个步骤还在后面定义了一个验证过程。
 *
 * @ingroup form_example
 */
function form_example_wizard_location_info($form, &$form_state) {
  $form = array();
  $form['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#description' => t('Hint: Do not enter "San Francisco", and do not leave this out.'),
    '#required' => TRUE,
    '#default_value' => !empty($form_state['values']['city']) ? $form_state['values']['city'] : '',

  );
  return $form;
}

/**
 * 'location info' 页面的自定义校验。
 *
 * 这是向导第二步的校验函数。输入值不能为空，也不能是 "San Francisco"。
 *
 * @ingroup form_example
 */
function form_example_wizard_location_info_validate($form, &$form_state) {
  if ($form_state['values']['city'] == 'San Francisco') {
    form_set_error('city', t('You were warned not to enter "San Francisco"'));
  }
}

/**
 * 为向导提供 ‘other info’ 的Form元素。
 * 这是这个向导的第三个也是最后一个步骤。
 *
 * @ingroup form_example
 */
function form_example_wizard_other_info($form, &$form_state) {
  $form = array();
  $form['aunts_name'] = array(
    '#type' => 'textfield',
    '#title' => t("Your first cousin's aunt's Social Security number"),
    '#default_value' => !empty($form_state['values']['aunts_name']) ? $form_state['values']['aunts_name'] : '',
  );
  return $form;
}

/**
 * 向导Form的提交处理。
 *
 * 本函数：
 * - 把 $form_state['values'] 保存到别处。
 * - 处理所有的Form内容。
 *
 * 接下来就是这个向导的神奇之处了，这个函数会处理用户在每个不同步骤中输入的不同内容。
 *
 * 这个演示处理函数只是在收集了所有步骤的输入之后，调用了一下 drupal_set_message()。
 *
 * @ingroup form_example
 */
function form_example_wizard_submit($form, &$form_state) {
  $current_step = &$form_state['step'];
  $form_state['step_information'][$current_step]['stored_values'] = $form_state['values'];

  // 这里我们结束了向导的最后一页，开始处理提交的信息。
  drupal_set_message(t('This information was collected by this wizard:'));
  foreach ($form_state['step_information'] as $index => $value) {
    // 移除 values 中的 Form API 字段（form_token, form_id 和 form_build_id）。
    // 这个步骤并不是必要的，你可以直接访问 $value['stored_values']。
    // 不过移除这些内容可以获得一个更清晰的结果，可以把整个数组传递给
    // drupal_set_message()。
    unset($value['stored_values']['form_id']);
    unset($value['stored_values']['form_build_id']);
    unset($value['stored_values']['form_token']);

    // 显示所有信息。
    drupal_set_message(t('Step @num collected the following values: <pre>@result</pre>',
      array(
        '@num' => $index,
        '@result' => print_r($value['stored_values'], TRUE),
      )
    ));
  }
}
