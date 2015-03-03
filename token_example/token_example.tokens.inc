<?php

/**
 * @file
 * Token callbacks for the token_example module.
 */

/**
 * Implements hook_token_info().
 *
 * @ingroup token_example
 */
function token_example_token_info() {
  //加入两种不同的Token类型：
  //  - 第一个是通用的文本格式；
  //  - 第二种是用户的缺省文本格式，这种是一个“格式”Token，可以直接使用

  $info['types']['format'] = array(
    'name' => t('Text formats'),
    'description' => t('Tokens related to text formats.'),
    'needs-data' => 'format',
  );
  $info['types']['default-format'] = array(
    'name' => t('Default text format'),
    'description' => t("Tokens related to the currently logged in user's default text format."),
    'type' => 'format',
  );

  // Tokens for the text format token type.
  // 文本格式Token。
  $info['tokens']['format']['id'] = array(
    'name' => t('ID'),
    'description' => t("The unique ID of the text format."),
  );
  $info['tokens']['format']['name'] = array(
    'name' => t('Name'),
    'description' => t("The name of the text format."),
  );

  // Node Token。
  $info['tokens']['node']['body-format'] = array(
    'name' => t('Body text format'),
    'description' => t("The name of the text format used on the node's body field."),
    'type' => 'format',
  );

  // 评论 Token。
  if (module_exists('comment')) {
    $info['tokens']['comment']['body-format'] = array(
      'name' => t('Body text format'),
      'description' => t("The name of the text format used on the comment's body field."),
      'type' => 'format',
    );
  }

  return $info;
}

/**
 * Implements hook_tokens().
 *
 * @ingroup token_example
 */
function token_example_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);

  // 文本格式Token。
  if ($type == 'format' && !empty($data['format'])) {
    $format = $data['format'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'id':
          // Since {filter_format}.format is an integer and not user-entered
          // text, it does not need to ever be sanitized.
          $replacements[$original] = $format->format;
          break;

        case 'name':
          // Since the format name is user-entered text, santize when requested.
          $replacements[$original] = $sanitize ? filter_xss($format->name) : $format->name;
          break;
      }
    }
  }

  // 缺省格式Token。
  if ($type == 'default-format') {
    $default_id = filter_default_format();
    $default_format = filter_format_load($default_id);
    $replacements += token_generate('format', $tokens, array('format' => $default_format), $options);
  }

  // Node Token。
  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'body-format':
          if ($items = field_get_items('node', $node, 'body')) {
            $format = filter_format_load($items[0]['format']);
            $replacements[$original] = $sanitize ? filter_xss($format->name) : $format->name;
          }
          break;
      }
    }

    // Chained token relationships.
    //todo:链式Token关系
    if ($format_tokens = token_find_with_prefix($tokens, 'body-format')) {
      if ($items = field_get_items('node', $node, 'body')) {
        $body_format = filter_format_load($items[0]['format']);
        $replacements += token_generate('format', $format_tokens, array('format' => $body_format), $options);
      }
    }
  }

  // 评论Token。
  if ($type == 'comment' && !empty($data['comment'])) {
    $comment = $data['comment'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'body-format':
          if ($items = field_get_items('comment', $comment, 'comment_body')) {
            $format = filter_format_load($items[0]['format']);
            $replacements[$original] = $sanitize ? filter_xss($format->name) : $format->name;
          }
          break;
      }
    }

    // Chained token relationships.
    //todo:链式Token关系
    if ($format_tokens = token_find_with_prefix($tokens, 'body-format')) {
      if ($items = field_get_items('comment', $comment, 'comment_body')) {
        $body_format = filter_format_load($items[0]['format']);
        $replacements += token_generate('format', $format_tokens, array('format' => $body_format), $options);
      }
    }
  }

  return $replacements;
}
