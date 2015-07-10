<?php

/**
 * @file
 * 这是一个用来演示如何定义Form，渲染元素的示例模块。
 *
 * Form 元素对任何使用 Form API 的用户来说都应该很熟悉。他跟render element类似，
 * 可以参照@link render_example.module Render Example @endlink，例子中的核心
 * Form元素是 ‘textfield’，‘checkbox’ 以及 ‘fieldset’。Drupal 实现了hook_elements
 * 来定义这些 Form API 类型，具体实现函数是 system_elements()。
 *
 * 每个 Form 元素都有一个 #type 值，这个值决定了 Form API 对他的处理方式，以及如何渲染
 * 成为 HTML。
 * 模块可以利用 hook_element_info() 来定义新的元素类型，并告知 Form API 这种元素的缺省
 * 值。
 *
 * 在模块中实现 hook_element_info()，可以创建和自己的 Form 元素，并根据他们的属性进行
 * 渲染、验证。
 *
 * 这个例子中，我们定义了一系列的元素，从一个改名的文本字段，到更复杂的电话
 * 号码字段——这个字段的每个部分会被分别进行验证。
 *
 * 既然每个元素都能够定义属性（例如 #process 或者 #dropthis），所以要指出每种属性的含义
 * 是很困难的，因此本例中不会详尽的解释所有属性。
 */

/**
 * @todo: 有些奇妙的做法需要解释：
 * - #process 和 process callback （以及命名） (在 Form 中)
 * - #value 和 value callback （同样也有命名)
 * - #theme 和 #theme_wrappers
 * - 什么是 #return_value？
 * - 系统模块提供了标准的缺省元素。
 * - hook_element_info() 中到底能定义些什么？缺省值从哪里来？
 * - Form 中具有匹配 hook_element_info() 中定义类型的的元素，会获取这些项目并进行合并。
 * - #value_callback 首先会被 form_builder() 调用。他的任务是确定元素的值，可能从
 *   #default_value 或者其他位置取得。
 * - #process 要允许对整个元素的变化，就可以调用 #process（例如添加一个子 Form 元素）。
 * - #return_value：需要为 Form API 提供三个不同的返回值：
 *   #default_value：缺省值；
 *   #return_value：如果该元素被选中；
 *   #value：0 或者 #return_value。
 */

/**
 * 工具函数，为 form_example_element_info() 提供数据。
 *
 * 这里定义了多种新的 Form 元素类型。
 *
 * - form_example_textfield：这实际上就是一个文本字段，只不过是做成了新类型。还可以
 *   为他提供主题函数。
 * - form_example_checkbox：跟普通的 checkbox 没太大区别，不同的是他使用了本模块自行
 *   实现的主题函数。
 * - form_example_phonenumber_discrete：提供了一个北美风格的三段式电话号码，用一个数
 * - 分别存储。
 * - form_example_phonenumber_combined：同样提供了三段式电话号码，这里用一个10位字符
 * - 串来保存值，仅在界面上分开三段。
 *
 * form_builder() 有很多 #process 和 #value_callback 方面的内容。
 * 还可以参看 hook_element_info().
 *
 * system_element_info() 包含了 Drupal 的缺省元素类型，也可以作为很好的实例。
 */
function _form_example_element_info() {
  // form_example_textfield 是一个基于 textfield 的元素，只为他准备了定义和主题函数。
  // 在这里，我们给他指派了文本字段的主题函数，实际上这个应该由 hook_theme() 提供，
  // "form_example_textfield"的主题函数实现，也就是
  // theme_form_example_textfield()。注意 'form_example_textfield' 元素类型完全在
  // 这里定义，无需更多代码支持了。

  $types['form_example_textfield'] = array(
    // #input = TRUE 就是说，输入的值会被当做 #value 处理。
    '#input' => TRUE,

    // 在输出时使用 theme('textfield') 来格式化。
    '#theme' => array('textfield'),

    // 不提供自动完成。
    '#autocomplete_path' => FALSE,

    // 输出时，允许 theme('form_element') 来控制外围的标记。
    '#theme_wrappers' => array('form_element'),
  );


  // form_example_checkbox 基本上就是系统定义的 checkbox 元素的拷贝。
  $types['form_example_checkbox'] = array(
    // HTML 输入元素。
    '#input' => TRUE,

    // @todo: 解释 #return_value.
    '#return_value' => TRUE,

    // 这里的 #process array 使用了普通 checkbox 的标准处理函数。
    '#process' => array('form_process_checkbox', 'ajax_process_form'),

    // 用 theme('form_example_checkbox') 来渲染和输出。
    '#theme' => 'form_example_checkbox',

    // 用 theme('form_element') 为本元素提供 HTML 包装。
    '#theme_wrappers' => array('form_element'),

    // 在元素后（也就是右边）放置标题。
    // 这个属性会影响到 theme_form_element() 的行为。
    '#title_display' => 'after'

    // '#value_callback' => 'form_type_form_example_checkbox_value',
    // 我们使用缺省的函数名作为 value callback， 所以这里不用显式的列出来。
    // 缺省函数命名方式为 form_type_TYPENAME_value()。
  );

  // 这个分段电话号码元素用分开三个元素：区号，前缀以及分机号的方式来保存值。
  $types['form_example_phonenumber_discrete'] = array(
    // #input == TRUE 就是说 Form 值就是 #value
    '#input' => TRUE,

    // #process 是一个 callback 函数数组，这些函数会在处理元素时候执行。
    // 这里提供了子 Form 元素，用于承载 区号，前缀和分机号。
    '#process' => array('form_example_phonenumber_discrete_process'),

    // 元素的验证函数。
    '#element_validate' => array('form_example_phonenumber_discrete_validate'),
    '#autocomplete_path' => FALSE,
    '#theme_wrappers' => array('form_example_inline_form_element'),
  );

  // 定义 form_example_phonenumber_combined，
  // 用于把电话号码合成一个单独的待验证字符串。
  $types['form_example_phonenumber_combined'] = array(
    '#input' => TRUE ,
    '#process' => array('form_example_phonenumber_combined_process'),
    '#element_validate' => array('form_example_phonenumber_combined_validate'),
    '#autocomplete_path' => FALSE,
    '#value_callback'   => 'form_example_phonenumber_combined_value',
    '#default_value' => array(
      'areacode' => '',
      'prefix' => '',
      'extension' => '',
    ),
    '#theme_wrappers' => array('form_example_inline_form_element'),
  );
  return $types;
}


/**
 * form_example_phonenumber_combined 的 value callback。
 *
 * 只有当 form builder 没有处理输入的时候，才为电话号码建立当前的联合值。
 *
 * @param array $element
 *   Form 元素。
 * @param array $input
 *   输入。
 * @param array $form_state
 *   Form state.
 *
 * @return array
 *   修改后的元素。
 */
function form_example_phonenumber_combined_value(&$element, $input = FALSE, $form_state = NULL) {
  if (!$form_state['process_input']) {
    $matches = array();
    $match = preg_match('/^(\d{3})(\d{3})(\d{4})$/', $element['#default_value'], $matches);
    if ($match) {
      // 去掉第一个全匹配的结果，然后分别赋值。
      array_shift($matches);
      list($element['areacode'], $element['prefix'], $element['extension']) = $matches;
    }
  }
  return $element;
}

/**
 * form_example_checkbox 元素类型的 value callback。
 *
 * 从 form_type_checkbox_value() 拷贝而来。
 *
 * @param array $element
 *   要运算值的 Form 元素。
 * @param mixed $input
 *   Form 元素中产生的待处理输入。 如果是 FALSE，意味着没有输入，会返回该元素的缺省值。
 *
 * @return int
 *   Form 元素所表达的值。
 */
function form_type_form_example_checkbox_value($element, $input = FALSE) {
  if ($input === FALSE) {
    return isset($element['#default_value']) ? $element['#default_value'] : 0;
  }
  else {
    return isset($input) ? $element['#return_value'] : 0;
  }
}

/**
 * 分隔式电话号码元素的 process callback。
 */
function form_example_phonenumber_discrete_process($element, &$form_state, $complete_form) {
  // #tree = TRUE 说明 $form_state['values'] 中会进行分级保存。
  // 在这里， 这个元素的各个部分在 $form_state['values'] 是这样存储的：
  // $form_state['values']['<element_name>']['areacode']，
  // $form_state['values']['<element_name>']['prefix']，等等。
  // 一般来说，如果元素具有子元素，会推荐这种方式。
  $element['#tree'] = TRUE;

  // 基本上是普通的 FAPI 字段定义，#value 稍有不同。
  $element['areacode'] = array(
    '#type' => 'textfield',
    '#size' => 3,
    '#maxlength' => 3,
    '#value' => $element['#value']['areacode'],
    '#required' => TRUE,
    '#prefix' => '(',
    '#suffix' => ')',
  );
  $element['prefix'] = array(
    '#type' => 'textfield',
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => TRUE,
    '#value' => $element['#value']['prefix'],
  );
  $element['extension'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#maxlength' => 4,
    '#value' => $element['#value']['extension'],
  );

  return $element;
}

/**
 * 分隔版本的电话号码元素的验证函数。
 *
 * 这里使用正则表达式验证如下内容：
 *  - 区号是三位数字。
 *  - 前缀是三位数字。
 *  - 分机号是四位数字。
 *
 * 所有出现的问题都用 form_error() 展示。
 */
function form_example_phonenumber_discrete_validate($element, &$form_state) {
  if (isset($element['#value']['areacode'])) {
    if (0 == preg_match('/^\d{3}$/', $element['#value']['areacode'])) {
      form_error($element['areacode'], t('The area code is invalid.'));
    }
  }
  if (isset($element['#value']['prefix'])) {
    if (0 == preg_match('/^\d{3}$/', $element['#value']['prefix'])) {
      form_error($element['prefix'], t('The prefix is invalid.'));
    }
  }
  if (isset($element['#value']['extension'])) {
    if (0 == preg_match('/^\d{4}$/', $element['#value']['extension'])) {
      form_error($element['extension'], t('The extension is invalid.'));
    }
  }
  return $element;
}

/**
 * 合并版本的电话号码元素的 Process callback。
 */
function form_example_phonenumber_combined_process($element, &$form_state, $complete_form) {
  // #tree = TRUE 说明 $form_state['values'] 中会进行分级保存。
  // 在这里， 这个元素的各个部分在 $form_state['values'] 是这样存储的：
  // $form_state['values']['<element_name>']['areacode']，
  // $form_state['values']['<element_name>']['prefix']，等等。
  // 一般来说，如果元素具有子元素，会推荐这种方式。
  $element['#tree'] = TRUE;

  // 基本上是普通的 FAPI 字段定义，#value 稍有不同。
  $element['areacode'] = array(
    '#type' => 'textfield',
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => TRUE,
    '#prefix' => '(',
    '#suffix' => ')',
  );
  $element['prefix'] = array(
    '#type' => 'textfield',
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => TRUE,
  );
  $element['extension'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#maxlength' => 4,
    '#required' => TRUE,
  );

  $matches = array();
  $match = preg_match('/^(\d{3})(\d{3})(\d{4})$/', $element['#default_value'], $matches);
  if ($match) {
    // 去掉第一个全匹配的结果，然后分别赋值。
    array_shift($matches);
    list($element['areacode']['#default_value'], $element['prefix']['#default_value'], $element['extension']['#default_value']) = $matches;
  }

  return $element;
}

/**
 * 分隔版本的电话号码元素的验证函数。
 *
 * 这里使用正则表达式验证如下内容：
 *  - 区号是三位数字。
 *  - 前缀是三位数字。
 *  - 分机号是四位数字。
 *
 * 所有出现的问题都用 form_error() 展示。
 * 然后会更新合并值。
 */
function form_example_phonenumber_combined_validate($element, &$form_state) {
  $lengths = array(
    'areacode' => 3,
    'prefix' => 3,
    'extension' => 4,
  );
  foreach ($lengths as $member => $length) {
    $regex = '/^\d{' . $length . '}$/';
    if (!empty($element['#value'][$member]) && 0 == preg_match($regex, $element['#value'][$member])) {
      form_error($element[$member], t('@member is invalid', array('@member' => $member)));
    }
  }

  // 把三个值合并为一个。
  $value = $element['areacode']['#value'] . $element['prefix']['#value'] . $element['extension']['#value'];
  form_set_value($element, $value, $form_state);
  return $element;
}

/**
 * 会被 form_example_theme() 调用，提供 hook_theme() 功能。
 *
 * 为了跟主题函数放在一起，所以保存在这个文件中。
 */
function _form_example_element_theme() {
  return array(
    'form_example_inline_form_element' => array(
      'render element' => 'element',
      'file' => 'form_example_elements.inc',
    ),
    'form_example_checkbox' => array(
      'render element' => 'element',
      'file' => 'form_example_elements.inc',
    ),
  );
}

/**
 * 渲染一个自定义的 checkbox 。
 *
 * 这里实际上什么都没做，只是来展示一下这里可以做什么。
 */
function theme_form_example_checkbox($variables) {
  $element = $variables['element'];
  return theme('checkbox', $element);
}

/**
 * 把子元素格式化为行内元素。
 */
function theme_form_example_inline_form_element($variables) {
  $element = $variables['element'];

  // 添加元素 #id。
  if (isset($element['#markup']) && !empty($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  // 为了方便使用 JS/CSS 选择器，把元素的 #type 和 #name 作为类。
  $attributes['class'] = array('form-item');
  if (!empty($element['#type'])) {
    $attributes['class'][] = 'form-type-' . strtr($element['#type'], '_', '-');
  }
  if (!empty($element['#name'])) {
    $attributes['class'][] = 'form-item-' . strtr($element['#name'],
      array(
        ' ' => '-',
        '_' => '-',
        '[' => '-',
        ']' => '',
      )
    );
  }
  // 为禁用的元素添加一个类，方便跨浏览器的兼容工作。
  if (!empty($element['#attributes']['disabled'])) {
    $attributes['class'][] = 'form-disabled';
  }
  $output = '<div' . drupal_attributes($attributes) . '>' . "\n";

  // 如果没有设置 #title， 就不显示标签。
  if (!isset($element['#title'])) {
    $element['#title_display'] = 'none';
  }
  $prefix = isset($element['#field_prefix']) ? '<span class="field-prefix">' . $element['#field_prefix'] . '</span> ' : '';
  $suffix = isset($element['#field_suffix']) ? ' <span class="field-suffix">' . $element['#field_suffix'] . '</span>' : '';

  switch ($element['#title_display']) {
    case 'before':
      $output .= ' ' . theme('form_element_label', $variables);
      $output .= ' ' . '<div class="container-inline">' . $prefix . $element['#children'] . $suffix . "</div>\n";
      break;

    case 'invisible':
    case 'after':
      $output .= ' ' . $prefix . $element['#children'] . $suffix;
      $output .= ' ' . theme('form_element_label', $variables) . "\n";
      break;

    case 'none':
    case 'attribute':
      // 只显示子元素，不显示标签。
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      break;
  }

  if (!empty($element['#description'])) {
    $output .= ' <div class="description">' . $element['#description'] . "</div>\n";
  }

  $output .= "</div>\n";

  return $output;
}

/**
 * examples/form_example/element_example 的 Form 内容。
 * 简单的一个Form ， 用来示范如何使用我们新定义的Form API 元素类型。
 */
function form_example_element_demo_form($form, &$form_state) {
  $form['a_form_example_textfield'] = array(
    '#type' => 'form_example_textfield',
    '#title' => t('Form Example textfield'),
    '#default_value' => variable_get('form_example_textfield', ''),
    '#description' => t('form_example_textfield is a new type, but it is actually uses the system-provided functions of textfield'),
  );

  $form['a_form_example_checkbox'] = array(
    '#type' => 'form_example_checkbox',
    '#title' => t('Form Example checkbox'),
    '#default_value' => variable_get('form_example_checkbox', FALSE),
    '#description' => t('Nothing more than a regular checkbox but with a theme provided by this module.'),
  );

  $form['a_form_example_element_discrete'] = array(
    '#type' => 'form_example_phonenumber_discrete',
    '#title' => t('Discrete phone number'),
    '#default_value' => variable_get(
      'form_example_element_discrete',
      array(
        'areacode' => '999',
        'prefix' => '999',
        'extension' => '9999',
      )
    ),
    '#description' => t('A phone number : areacode (XXX), prefix (XXX) and extension (XXXX). This one uses a "discrete" element type, one which stores the three parts of the telephone number separately.'),
  );

  $form['a_form_example_element_combined'] = array(
    '#type' => 'form_example_phonenumber_combined',
    '#title' => t('Combined phone number'),
    '#default_value' => variable_get('form_example_element_combined', '0000000000'),
    '#description' => t('form_example_element_combined one uses a "combined" element type, one with a single 10-digit value which is broken apart when needed.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * form_example_element_demo_form() 的提交处理。
 */
function form_example_element_demo_form_submit($form, &$form_state) {
  // 排除无用元素。
  unset($form_state['values']['submit'], $form_state['values']['form_id'], $form_state['values']['op'], $form_state['values']['form_token'], $form_state['values']['form_build_id']);

  foreach ($form_state['values'] as $key => $value) {
    variable_set($key, $value);
    drupal_set_message(
      t('%name has value %value',
        array(
          '%name' => $key,
          '%value' => print_r($value, TRUE),
        )
      )
    );
  }
}
