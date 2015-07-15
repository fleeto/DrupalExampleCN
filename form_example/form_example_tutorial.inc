<?php

/**
 * @file
 * This is the Form API Tutorial from the handbook.
 *
 * It goes through several form examples of increasing complexity to demonstrate
 * Drupal 7 Form API.
 *
 * Links are provided inline for the related handbook pages.
 *
 * @see http://drupal.org/node/262422
 */

/**
 * Main Form tutorial page.
 *
 * @see form_example_tutorial_1()
 * @see form_example_tutorial_2()
 * @see form_example_tutorial_3()
 * @see form_example_tutorial_4()
 * @see form_example_tutorial_5()
 * @see form_example_tutorial_6()
 * @see form_example_tutorial_7()
 * @see form_example_tutorial_8()
 * @see form_example_tutorial_9()
 * @see form_example_tutorial_10()
 *
 * @ingroup form_example
 */
function form_example_tutorial() {
  return t('This is a set of form tutorials tied to the <a href="http://drupal.org/node/262422">Drupal handbook</a>.');
}

/**
 * 示例 1。
 *
 * 第一个Form例子来自于
 * @link Form教学手册。 http://drupal.org/node/717722  @endlink
 *
 * 这里仅仅是创建一个非常基础的带有一个文本字段的窗体。
 *
 * 这个函数可称为窗体生成函数，用于创建窗体。
 * 这一函数接收两个参数，$form 和 $form_state，但是如果drupal_get_form()发送了额外的
 * 参数，这诶参数会在$form_state之后出现。
 *
 * @ingroup form_example
 */
function form_example_tutorial_1($form, &$form_state) {

  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('A form with nothing but a textfield'),
  );
  // 这是第一个Form元素，一个带有标签的文本字段，"Name"
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
  );
  return $form;
}

/**
 * 示例 2：一个带有提交按钮的基础窗体。
 *
 * @see http://drupal.org/node/717726
 * @ingroup form_example
 */
function form_example_tutorial_2($form, &$form_state) {
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('A simple form with a submit button'),
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
  );

  // 添加一个提交按钮:刷新窗体，清空内容。这是窗体的基础行为。
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}

/**
 *
 * 示例 3：一个带有字段集（fieldset）的基础窗体。
 *
 * 我们声明了一个字段集元素，并把两个文本字段置入其中，其中一个是姓氏，一个是名字。
 * 字段集用于组织具有相关性的内容。
 *
 * 下面的代码，你会注意到我们把姓氏和名字两个字段放到了$form['name']的下级。
 * 对Form API来说，这种赋值就意味着把这两个字段放入 $form['name'] 这个字段集中。
 *
 * @ingroup form_example
 */
function form_example_tutorial_3($form, &$form_state) {
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('A form with a fieldset'),
  );

  $form['name'] = array(
    '#type' => 'fieldset',
    '#title' => t('Name'),
  );
  $form['name']['first'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
  );
  $form['name']['last'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}

/**
 * 示例 4：带有必填字段的简单 Form。
 *
 * @ingroup form_example
 */
function form_example_tutorial_4($form, &$form_state) {
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('A form with required fields'),
  );

  $form['name'] = array(
    '#type' => 'fieldset',
    '#title' => t('Name'),
    // 让字段集可以被折叠。
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // 下面的字段属必填字段
  $form['name']['first'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#required' => TRUE,
  );
  $form['name']['last'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}

/**
 *
 * 第五个例子：带有附加元素属性的基础窗体。
 *
 *
 * 演示文本字段的附加属性。
 *
 * 参见
 * @link http://api.drupal.org/api/file/developer/topics/forms_api.html complete form reference @endlink
 *
 * @ingroup form_example
 */
function form_example_tutorial_5($form, &$form_state) {
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('A form with additional attributes'),
    '#description' => t('This one adds #default_value and #description'),
  );
  $form['name'] = array(
    '#type' => 'fieldset',
    '#title' => t('Name'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['name']['first'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#required' => TRUE,
    '#default_value' => "First name",
    '#description' => "Please enter your first name.",
    '#size' => 20,
    '#maxlength' => 20,
  );
  $form['name']['last'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}

/**
 *
 * 示例 6：一个带有校验处理的基础Form。
 *
 * 来自于 http://drupal.org/node/717736
 * @see form_example_tutorial_6_validate()
 *
 * @ingroup form_example
 */
function form_example_tutorial_6($form, &$form_state) {
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('A form with a validation handler'),
  );

  $form['name'] = array(
    '#type' => 'fieldset',
    '#title' => t('Name'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['name']['first'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#required' => TRUE,
    '#default_value' => "First name",
    '#description' => "Please enter your first name.",
    '#size' => 20,
    '#maxlength' => 20,
  );
  $form['name']['last'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#required' => TRUE,
  );

  // 新增了一个字段用于输入出生年份。
  // 输入到这一字段的数据会被缺省的验证函数进行验证。
  $form['year_of_birth'] = array(
    '#type' => 'textfield',
    '#title' => "Year of birth",
    '#description' => 'Format is "YYYY"',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}

/**
 *
 * 第六个例子中用到的校验处理。
 *
 * Now we add a handler/function to validate the data entered into the
 * "year of birth" field to make sure it's between the values of 1900
 * and 2000. If not, it displays an error. The value report is
 * $form_state['values'] (see http://drupal.org/node/144132#form-state).
 *
 * 现在我们添加了一个处理函数，来校验输入到出声年份字段中的数据，以此确保他的值在
 * 1900和2000之间。如果超出范围，则显示错误信息。值保存于$form_state['values']
 * （参考 http://drupal.org/node/144132#form-state）。
 *
 *
 * 注意函数的名字——由窗体名称加上'_validate'后缀组合而成。缺省的校验函数都是如此命名的。
 * 还可以在$form['#validate']中以列表的形式提供多个验证函数。
 *
 * @see form_example_tutorial_6()
 *
 * @ingroup form_example
 */
function form_example_tutorial_6_validate($form, &$form_state) {
  $year_of_birth = $form_state['values']['year_of_birth'];
  if ($year_of_birth && ($year_of_birth < 1900 || $year_of_birth > 2000)) {
    form_set_error('year_of_birth', t('Enter a year between 1900 and 2000.'));
  }
}

/**
 *
 * 示例 7：提交处理。
 *
 * 来自于手册：
 * http://drupal.org/node/717740
 *
 * @see form_example_tutorial_7_validate()
 * @see form_example_tutorial_7_submit()
 *
 * @ingroup form_example
 */
function form_example_tutorial_7($form, &$form_state) {
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('A form with a submit handler'),
  );
  $form['name'] = array(
    '#type' => 'fieldset',
    '#title' => t('Name'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['name']['first'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#required' => TRUE,
    '#default_value' => "First name",
    '#description' => "Please enter your first name.",
    '#size' => 20,
    '#maxlength' => 20,
  );
  $form['name']['last'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#required' => TRUE,
  );
  $form['year_of_birth'] = array(
    '#type' => 'textfield',
    '#title' => "Year of birth",
    '#description' => 'Format is "YYYY"',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}


/**
 * form_example_tutorial_7() 的验证函数。
 *
 * @ingroup form_example
 */
function form_example_tutorial_7_validate($form, &$form_state) {
  $year_of_birth = $form_state['values']['year_of_birth'];
  if ($year_of_birth && ($year_of_birth < 1900 || $year_of_birth > 2000)) {
    form_set_error('year_of_birth', t('Enter a year between 1900 and 2000.'));
  }
}

/**
 *
 * form_example_tutorial_7() 的提交函数。
 *
 * 为窗体添加一个提交处理函数，用来向屏幕输出一个成功结束的消息。
 *
 * @ingroup form_example
 */
function form_example_tutorial_7_submit($form, &$form_state) {
  drupal_set_message(t('The form has been submitted. name="@first @last", year of birth=@year_of_birth',
    array(
      '@first' => $form_state['values']['first'],
      '@last' => $form_state['values']['last'],
      '@year_of_birth' => $form_state['values']['year_of_birth'],
    )
  ));
}

/**
 *
 * 示例 8：一个简单的，带有前进和后退按钮的多步骤窗体。
 *
 * 手册: http://drupal.org/node/717750.
 *
 * @link form_example_wizard.inc form_example_wizard.inc @endlink
 * 提供了扩展性更好的多步骤窗体示例。
 *
 *
 * 为窗体构建器添加了一个逻辑，用来提供两个页面。
 *
 * @link ajax_example_wizard AJAX Example's Wizard Example @endlink
 * 提供了同样功能的AJAX版本。
 *
 * @see form_example_tutorial_8_page_two()
 * @see form_example_tutorial_8_page_two_back()
 * @see form_example_tutorial_8_page_two_submit()
 * @see form_example_tutorial_8_next_submit()
 * @see form_example_tutorial.inc
 *
 * @ingroup form_example
 */
function form_example_tutorial_8($form, &$form_state) {


  // 如果$form_state['page_num'] 为2，则显示第二页。
  if (!empty($form_state['page_num']) && $form_state['page_num'] == 2) {
    return form_example_tutorial_8_page_two($form, $form_state);
  }

  // 否则显示第一页。
  $form_state['page_num'] = 1;

  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('A basic multistep form (page 1)'),
  );

  $form['first'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#description' => "Please enter your first name.",
    '#size' => 20,
    '#maxlength' => 20,
    '#required' => TRUE,
    '#default_value' => !empty($form_state['values']['first']) ? $form_state['values']['first'] : '',
  );
  $form['last'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#default_value' => !empty($form_state['values']['last']) ? $form_state['values']['last'] : '',
  );
  $form['year_of_birth'] = array(
    '#type' => 'textfield',
    '#title' => "Year of birth",
    '#description' => 'Format is "YYYY"',
    '#default_value' => !empty($form_state['values']['year_of_birth']) ? $form_state['values']['year_of_birth'] : '',
  );
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => 'Next >>',
    '#submit' => array('form_example_tutorial_8_next_submit'),
    '#validate' => array('form_example_tutorial_8_next_validate'),
  );
  return $form;
}

/**
 * form_example_tutorial_8() 中的第二页.
 *
 * @ingroup form_example
 * @param $form
 * @param $form_state
 * @return
 */
function form_example_tutorial_8_page_two($form, &$form_state) {
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('A basic multistep form (page 2)'),
  );

  $form['color'] = array(
    '#type' => 'textfield',
    '#title' => t('Favorite color'),
    '#required' => TRUE,
    '#default_value' => !empty($form_state['values']['color']) ? $form_state['values']['color'] : '',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => array('form_example_tutorial_8_page_two_submit'),
  );
  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('<< Back'),
    '#submit' => array('form_example_tutorial_8_page_two_back'),

    // 因为最终要回到这页进行提交，所以我们不用担心 'color' 字段的验证，
    '#limit_validation_errors' => array(),
  );
  return $form;
}


/**
 * 第一页的“下一步”按钮的验证函数
 *
 * @ingroup form_example
 * @param $form
 * @param $form_state
 */
function form_example_tutorial_8_next_validate($form, &$form_state) {
  $year_of_birth = $form_state['values']['year_of_birth'];
  if ($year_of_birth && ($year_of_birth < 1900 || $year_of_birth > 2000)) {
    form_set_error('year_of_birth', t('Enter a year between 1900 and 2000.'));
  }
}

/**
 *
 * form_example_tutorial_8() 的“下一步”按钮的提交处理。
 *
 * 从第一页抓取内容并保存起来，以便最终提交时使用。
 *
 * @ingroup form_example
 */
function form_example_tutorial_8_next_submit($form, &$form_state) {

  // 每一页的数据都会被保存起来，
  // 并传递给窗体的其余页面。
  // 通知FAPI重建窗体。
  $form_state['page_values'][1] = $form_state['values'];

  if (!empty($form_state['page_values'][2])) {
    $form_state['values'] = $form_state['page_values'][2];
  }

  // 当窗体重建时，会查看这个值，来确定重建哪页。
  $form_state['page_num'] = 2;
  $form_state['rebuild'] = TRUE;
}

/**
 * 后退按钮的提交处理。
 *
 * 由于设置了#limit_validation_errors = array()，第二页的值会被丢弃，只会载入第一页
 * 的值。
 *
 * @ingroup form_example
 */
function form_example_tutorial_8_page_two_back($form, &$form_state) {
  $form_state['values'] = $form_state['page_values'][1];
  $form_state['page_num'] = 1;
  $form_state['rebuild'] = TRUE;
}

/**
 *
 * 第二页的提交处理。
 *
 * 这是最终的提交处理。收集所有的数据，并利用drupal_set_message()进行输出。
 *
 * @ingroup form_example
 */
function form_example_tutorial_8_page_two_submit($form, &$form_state) {
  // 通常情况下，这里应该是一些利用窗体数据来进行数据库操作的代码，而不是简单的一个
  // drupal_set_message()，这里只是用这一函数来检验是否正确运行。
  $page_one_values = $form_state['page_values'][1];
  drupal_set_message(t('The form has been submitted. name="@first @last", year of birth=@year_of_birth',
    array(
      '@first' => $page_one_values['first'],
      '@last' => $page_one_values['last'],
      '@year_of_birth' => $page_one_values['year_of_birth'],
    )
  ));

  if (!empty($page_one_values['first2'])) {
    drupal_set_message(t('Second name: name="@first @last", year of birth=@year_of_birth',
      array(
        '@first' => $page_one_values['first2'],
        '@last' => $page_one_values['last2'],
        '@year_of_birth' => $page_one_values['year_of_birth2'],
      )
    ));
  }
  drupal_set_message(t('And the favorite color is @color', array('@color' => $form_state['values']['color'])));

  // 如果需要在提交过程中进行重定向，可以设置 $form_state['redirect']。
  // 简单用法：这个值可以设置为一个字符串，代表重定向目标路径。
  // 例如重定向到 /node，可以这样做：

  // $form_state['redirect'] = 'node';
  //
  // 复杂一点的重定向，可以设置为一个数组，包含了重定向的参数，这些参数最后会
  // 传递给drupal_goto()。
  // 例如重定向到 /foo?bar=1#baz：
  //
  // @code
  // $form_state['redirect'] = array(
  //   'foo',
  //   array(
  //     'query' => array('bar' => 1),
  //     'fragment' => 'baz',
  //   ),
  // );
  // @endcode
  //
  // 数组的第一个元素是要重定向的目标，第二个元素是选项。可以参考
  // http://api.drupal.org/url获取更详细的参数信息。
}

/**
 * 示例 9：带有动态添加字段的Form
 *
 * 这个例子会添加一个缺省值，这样在Form重建时，就会获取到之前输入的值。
 *
 * Form手册 http://drupal.org/node/717746.
 *
 * @see form_example_tutorial_9_add_name()
 * @see form_example_tutorial_9_remove_name()
 * @see form_example_tutorial_9_submit()
 * @see form_example_tutorial_9_validate()
 *
 * @ingroup form_example
 */
function form_example_tutorial_9($form, &$form_state) {

  // 我们有很多同名的字段，所以我们要分层访问。
  $form['#tree'] = TRUE;

  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('A form with dynamically added new fields'),
  );

  if (empty($form_state['num_names'])) {
    $form_state['num_names'] = 1;
  }

  // 根据$form_state['num_names']，创建fieldset的名字。
  for ($i = 1; $i <= $form_state['num_names']; $i++) {
    $form['name'][$i] = array(
      '#type' => 'fieldset',
      '#title' => t('Name #@num', array('@num' => $i)),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['name'][$i]['first'] = array(
      '#type' => 'textfield',
      '#title' => t('First name'),
      '#description' => t("Enter first name."),
      '#size' => 20,
      '#maxlength' => 20,
      '#required' => TRUE,
    );
    $form['name'][$i]['last'] = array(
      '#type' => 'textfield',
      '#title' => t('Enter Last name'),
      '#required' => TRUE,
    );
    $form['name'][$i]['year_of_birth'] = array(
      '#type' => 'textfield',
      '#title' => t("Year of birth"),
      '#description' => t('Format is "YYYY"'),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  // 添加一个"加入一个名字"的按钮。
  $form['add_name'] = array(
    '#type' => 'submit',
    '#value' => t('Add another name'),
    '#submit' => array('form_example_tutorial_9_add_name'),
  );


  // 如果不止一个名字，这个按钮可以移除最后一个。
  if ($form_state['num_names'] > 1) {
    $form['remove_name'] = array(
      '#type' => 'submit',
      '#value' => t('Remove latest name'),
      '#submit' => array('form_example_tutorial_9_remove_name'),
      // 因为是要移除名字，所以暂时不做校验。
      '#limit_validation_errors' => array(),
    );
  }

  return $form;
}

/**
 * form_example_tutorial_9() 中"加入一个名字"按钮的提交处理。
 * $form_state['num_names'] 会通知Form创建函数，需要创建多少个fieldset。
 * 所以这里我们给他加一。
 *
 * 所有的$form_state元素都是持久的，所以不需要用什么特别的键，
 * 而过去是需要使用$form_state['storage']的。
 * 这里我们只是使用$form_state['num_names']就可以了。
 *
 * @ingroup form_example
 * @param $form
 * @param $form_state
 */

function form_example_tutorial_9_add_name($form, &$form_state) {
  // $form_state都是持久的，所以只需要使用$form_state['add_name']就可以了。
  $form_state['num_names']++;

  // 设置$form_state['rebuild'] = TRUE，会让Form重建。
  $form_state['rebuild'] = TRUE;
}

/**
 * form_example_tutorial_9()中"移除名称"按钮的提交处理。
 * @ingroup form_example
 */
function form_example_tutorial_9_remove_name($form, &$form_state) {
  if ($form_state['num_names'] > 1) {
    $form_state['num_names']--;
  }

  $form_state['rebuild'] = TRUE;
}

/**
 * form_example_tutorial_9()的验证函数。
 *
 * 如果存在新的字段，则加入一个验证逻辑进行检查。
 *
 * @ingroup form_example
 */
function form_example_tutorial_9_validate($form, &$form_state) {

  for ($i = 1; $i <= $form_state['num_names']; $i++) {
    $year_of_birth = $form_state['values']['name'][$i]['year_of_birth'];

    if ($year_of_birth && ($year_of_birth < 1900 || $year_of_birth > 2000)) {
      form_set_error("name][$i][year_of_birth", t('Enter a year between 1900 and 2000.'));
    }
  }
}

/**
 * form_example_tutorial_9()的提交函数。
 *
 * @ingroup form_example
 */
function form_example_tutorial_9_submit($form, &$form_state) {
  $output = t("Form 9 has been submitted.");
  for ($i = 1; $i <= $form_state['num_names']; $i++) {
    $output .= t("@num: @first @last (@date)...",
      array(
        '@num' => $i,
        '@first' => $form_state['values']['name'][$i]['first'],
        '@last' => $form_state['values']['name'][$i]['last'],
        '@date' => $form_state['values']['name'][$i]['year_of_birth'],
      )
    ) . ' ';
  }
  drupal_set_message($output);
}

/**
 * 示例10：一个带有文件字段的Form。
 *
 * 这个实例让用户可以上传一个文件到Drupal，文件会被存储到服务器，并在数据库中留下引用记录。
 *
 * @see form_example_tutorial_10_submit()
 * @see form_example_tutorial_10_validate()
 *
 * @ingroup form_example
 */
function form_example_tutorial_10($form_state) {

  // 如果你对浏览器的文件处理有些了解的话，会知道需要enctype="multipart/form-data。
  // Drupal会处理这个细节，无需我们关心。


  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Image'),
    '#description' => t('Upload a file, allowed extensions: jpg, jpeg, png, gif'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * form_example_tutorial_10()的校验过程。
 *
 * @ingroup form_example
 */
function form_example_tutorial_10_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    // 校验文件是否是一个图片
    'file_validate_is_image' => array(),
    // 检查扩展名。
    'file_validate_extensions' => array('png gif jpg jpeg'),
  ));
  // 如果通过了校验：
  if ($file) {
    // 把文件转到Drupal文件系统。
    if ($file = file_move($file, 'public://')) {
      // 把文件保存起来供提交过程处理。
      $form_state['storage']['file'] = $file;
    }
    else {
      form_set_error('file', t("Failed to write the uploaded file to the site's file folder."));
    }
  }
  else {
    form_set_error('file', t('No file was uploaded.'));
  }
}

/**
 * form_example_tutorial_10 的提交处理。
 *
 * @ingroup form_example
 */
function form_example_tutorial_10_submit($form, &$form_state) {
  $file = $form_state['storage']['file'];
  // 我们处理好了文件，从storage中移除。
  unset($form_state['storage']['file']);
  // 把文件永久保存。
  $file->status = FILE_STATUS_PERMANENT;
  // 保存状态。
  file_save($file);
  // 反馈给用户。
  drupal_set_message(t('The form has been submitted and the image has been saved, filename: @filename.', array('@filename' => $file->filename)));
}

/**
 * 示例11：添加一个确认Form。
 *
 * 这个例子生成一个简单的Form，当提交时，会把用户重定向到一个利用confirm_form函数
 * 生成的确认Form去。
 * 这个确认Form会询问用户输入是否正确。
 *
 * @see form_example_tutorial_11_submit()
 *
 * @ingroup form_example
 */
function form_example_tutorial_11($form, &$form_state) {
  // 这个Form和示例2基本一致，不同的是：我们在这里添加了一个#action标记，
  // 把提交过程重定向到一个确认页。
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('A set of two forms that demonstrate the confirm_form function.  This form has an explicit action to direct the form to a confirmation page'),
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}

/**
 * form_example_tutorial_11() 的提交处理。
 *
 * 为这个Form添加一个提交处理，用来把用户重定向到确认页面。
 *
 * @ingroup form_example
 */
function form_example_tutorial_11_submit($form, &$form_state) {
  // 简单的提交函数，根据Name字段修改重定向。
  $name = $form_state['values']['name'];
  $form_state['redirect'] = 'examples/form_example/tutorial/11/confirm/' . urlencode($name);
}

/**
 * 示例11，一个 confirm_form() 生成的form。
 * 这个函数利用 confirm_form() 生成一个确认 Form。
 * 如果确认则生成一个 Drupal 信息进行显示。
 *
 * @param string $name
 *   用 urlencode 处理过的 name。
 *
 * @see form_example_tutorial_11_confirm_name_submit()
 *
 * @ingroup form_example
 */
function form_example_tutorial_11_confirm_name($form, $form_state, $name) {
  // confirm_form() 返回一个完整的Form，用于确认一个动作。
  // 它具有7个参数：$form, $form, $question, $path, $description, $yes, $no, 以及
  // $name
  // - $form: 要添加到 Form 中的额外元素，会在提交处理中用到。
  // - $question: 确认页标题，用于提示用户要确认的内容。
  // - $path: 如果用户选择取消，页面转向的目标。
  // - $description = NULL: 额外的显示信息。
  // - $yes = NULL: 确认按钮的文本，缺省为 t('Confirm')。
  // - $no = NULL: 取消按钮的文本，缺省为 t('Cancel')。
  // - $name = 'confirm': 用于引用确认项的内部名称。



  // 首先我们为用户名建立一个文本字段。confirm_form()允许向确认窗体中添加元素，
  // 我们来试试
  $user_name_text_field = array(
    'name' => array(
      '#type' => 'textfield',
      // 这里我们不想用户编辑。
      '#disabled' => TRUE,
      '#title' => t('Your name:'),
      '#value' => urldecode($name),
    ),
  );

  // 询问用户的问题。
  $confirmation_question = t('Is this really your name?');

  // 如果用户选择取消，则转向这里。
  $cancel_path = 'examples/form_example/tutorial/11';

  // 一些有用的描述信息。
  $description = t('Please verify whether or not you have input your name correctly. If you verify you will be sent back to the form and a message will be set. Otherwise you will be sent to the same page but with no message.');

  // These are the text for our yes and no buttons.
  $yes_button = t('This is my name');
  $no_button = t('Nope, not my name');

  // Form API将使用这个名称来引用我们的确认Form。
  $confirm_name = 'confirm_example';

  // 最后，调用confirm_form()，会返回Form数组。
  return confirm_form(
    $user_name_text_field,
    $confirmation_question,
    $cancel_path,
    $description,
    $yes_button,
    $no_button,
    $confirm_name
  );
}

/**
 * form_example_tutorial_11_confirm_form() 的提交函数。
 *
 * 为确认form添加一个提交处理。如果得到了确认，我们将显示一个成功信息。
 *
 * @ingroup form_example
 */
function form_example_tutorial_11_confirm_name_submit($form, &$form_state) {
  drupal_set_message(t("Confirmation form submission recieved. According to your submission your name is '@name'", array("@name" => $form_state['values']['name'])));
  $form_state['redirect'] = 'examples/form_example/tutorial/11';
}
