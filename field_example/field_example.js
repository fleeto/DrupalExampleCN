/**
 * @file
 * 字段示例的Javascript
 */

/**
 * 给微件提供一个farbtastic颜色拾取器。
 */
(function ($) {
  Drupal.behaviors.field_example_colorpicker = {
    attach: function(context) {
      $(".edit-field-example-colorpicker").live("focus", function(event) {
        var edit_field = this;
        var picker = $(this).closest('div').parent().find(".field-example-colorpicker");

        // 隐藏所有其他的颜色拾取器。
        $(".field-example-colorpicker").hide();
        $(picker).show();
        $.farbtastic(picker, function(color) {
          edit_field.value = color;
        }).setColor(edit_field.value);
      });
    }
  }
})(jQuery);
