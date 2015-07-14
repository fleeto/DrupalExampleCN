<?php
/**
 * @file
 * Theming example 文本 Form 的模板文件。
 *
 * 可用的变量：
 * - $text_form: 一个包含了预渲染 Form 的字符串。
 * - $text_form_content: 一个键值对数组，以元素名称为键。
 *
 * 下面的缺省示例使用 Drupal 提供的缺省顺序对 Form 和其中的元素进行了渲染。
 *
 * 还可以按照自己设计的顺序输出每一个 Form 元素，并且添加额外的 HTML 标记来装饰 Form：
 * <?php print $text_form_content['element_name']; ?>
 *
 * 下面的代码会把 $text_form_content 数组的内容输出到页面源码中兵隐藏起来，可以由此
 * 查看单独的元素名称。
 *
 * <?php print '<!--' . print_r($text_form_content, TRUE) . '-->'; ?>
 */

?>

<!-- theming-example-text-form template -->
<div class="container-inline">
<?php  print $text_form; ?>
</div>
<!-- /theming-example-text-form template -->
