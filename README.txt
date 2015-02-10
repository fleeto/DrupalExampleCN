Examples for Developers
=======================

http://drupal.org/project/examples

这是什么?
-------------

这一系列模块尝试提供可用的Drupal特性和API的实例。我们力图使得模块代码简单、文档完善并且容易修改，以便开发者能够更快的学习到工作相关的深入内容。

这些例子希望能教给用户在Drupal 7下代码级开发的内容。有些方案可能使用第三方模块能够更好的完成任务，所以不要总是尝试直接使用PHP造轮子。

如何使用这些例子？
-----------------------

有三种主要的方式来使用本项目中的模块：

1. 在Drupal中启用和使用这些模块。不是所有的模块都有明显可见的效果。例如Page和Form API的例子会展示Form，数据库API的例子就不会显示什么东西。

2. 阅读代码。我们花费了巨大的精力用于提高代码自身、内部注释以及文档块的可读性


3. 在Web上浏览代码和文档。主要有两个途径：

* https://api.drupal.org/api/examples 是所有Drupal的主要API站点。这里能找到实例代码、相关API的所有交叉引用的各种资源。

* http://drupalcode.org/project/examples.git 这里可以浏览Examples项目的GIT仓库。


如何安装？
--------------------------

1. 为开发者准备Examples(解压缩到Drupal下的sites/all/modules)  


2. 在 Admin menu > Site building > Modules （url：admin/modules）菜单

3. 如果需要的话，重建访问权限。

4. 例子会显示在导航菜单上 (缺省在左边栏，如果禁用的话，需要重新启用).

现在你可以阅读代码和其中的注释，进行试验，并尽快掌握其中的原理。

如果你发现了问题，不正确的注释，过时的或者淘汰的代码，以及其他一些不合适的情况，可以去http://drupal.org/project/issues/examples搜索有没有相关Issue，如果没有，请新建一个。