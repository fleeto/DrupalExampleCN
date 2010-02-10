// $Id$

(function ($) {

/**
 * Custom summary for the module vertical tab. It will show "Not defined" untill
 * a custom string is entered in the textfield.
 */
Drupal.behaviors.vertical_tabs_exampleFieldsetSummaries = {
  attach: function (context) {
    // Use the fieldset class to identify the vertical tab element
    $('fieldset.vertical_tabs_example-custom-form', context).setSummary(function (context) {
      // Depending on the checkbox status, the settings will be customized, so
      // update the summary with the custom setting textfield string or a use a
      // default string.
      if ($('#edit-vertical_tabs_example-enabled', context).attr('checked')) {
        return Drupal.checkPlain($('#edit-vertical_tabs_example-custom-setting', context).val());
      }
      else {
        return Drupal.t('Using default.');
      }
    });
  }
};

})(jQuery);
