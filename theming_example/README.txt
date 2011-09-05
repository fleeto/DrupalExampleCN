The theming_example module attempts to show how module developers can add
theme functions to their projects so that themes can modify output.

Module developers should to strive to avoid hard-wiring any HTML into the output
of their code, this should all be done in theme functions.

Starting with the first example, function 'theming_example_page()':
the output is put into an array $content which is then fed to a theming
function 'theme_theming_example_page()' which loops over the content,
wrapping it in html in the process.

In order to get function 'theme_theming_example_page()' recognized it needs
to be registered with the module theme register function of the
type 'hook_theme'.

function 'theming_example_theme()' does this for this module.
for details of what can be done in this hook see the link to api.drupal.org

The functions 'theming_example_list_page()' and theming_example_order_form()
work in the same way.

In 'theme_theming_example_list_page()' the content is themed as an
ordered list and given a class attribute 'theming_example_mylist' which
is defined in theming_example.css

In function 'theme_theming_example_order_form()' the title is loaded into a
temporary variable '$title', deleted from the $form array and output
wrapped in html. The rest of the form is wrapped in a div using '#prefix'
and '#suffix'

The theming functions can be copied to a theme's template.php, renaming
appropriately, so if you theme is called 'mytheme' you would copy
function 'theme_theming_example_page()'
to
function 'mytheme_theming_example_page()' in template.php and it will be
used instead of the original.

The fourth example shows the use of a template file
'theming_example_text_form.tpl.php'
This file can be copied to a theme's folder and it will be used instead.

This example also shows what can be done using Drupal's
template_preprocess_HOOK method. In this case it modifies the output so
that a themer can output the whole form or gain control over some of its
parts in the template file.

