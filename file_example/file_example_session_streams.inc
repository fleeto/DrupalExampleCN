<?php
/**
 * @file
 * 实例：用于处理 session:// 模式的 Stream Wrapper。
 *
 * 这个例子是一个接近完整的实现，不过只是一个例子，还没有实际的应用。
 */

/**
 * 实例：用于处理 session:// 模式的 Stream Wrapper。
 *
 * 这是一个例子，如果在 $_SESSION 变量中存储太多东西，可能导致严重后果。这里用于演示
 * 使用 Stream Wrapper 进行读写的实现。
 *
 * “Stream” 是 Unix 世界中的一个重要概念，用于读写文件以及其他设备。对流进行读写，意味着
 * 打开某些设备、文件、网站或者其他什么东西，而不需要知道具体实现细节。所有的相关函数都是
 * 类似的，可以对流进行读写，在流中进行定位，以及其他一些操作，而无需得知其中的细节，甚至
 * 不用知道具体的目标设备。现在这一概念也在 PHP 中得到了实现。
 * "Stream Wrapper" 的用意是可任意扩展。经典案例就是PHP内置的 http:// 模式，可以用
 * file_get_contents("http://drupal.org/projects") 这样类似文件操作的方式来访问
 * 网站。而 Drupal 中添加了 public// 和 private:// 两个模式，还可以在模块中自行实现
 * 其他模式。本例中就加入了 session:// 模式，用这一模式可以用文件的方式来读写
 * $_SESSION['file_example']。
 *
 * 注意这个例子中的实现用的是简单的 PHP 数组，因此有个限制就是只能使用字符串的值，
 * 二进制内容无法正常工作。
 * @ingroup file_example
 */
class FileExampleSessionStreamWrapper implements DrupalStreamWrapperInterface {
  /**
   * Stream 上下文。
   *
   * @var Resource
   */
  public $context;


  /**
   * 实例的 URI (stream).
   *
   * 这个 Stream 使用 "session://example_target" 的模式来引用。
   *
   * @var String
   */
  protected $uri;

  /**
   * Stream 的内容
   *
   * 例子中只是用了 $_SESSION 变量，这只是一个简单的对 $_SESSION['file_example']
   * 的相关部分的引用。
   */
  protected $sessionContent;

  /**
   * 一个用于指示当前目录的指针。
   */
  protected $directoryPointer;

  /**
   * 指定目录中的键列表。
   */
  protected $directoryKeys;

  /**
   * 指向 Session 中下一个读写目标的指针。
   */
  protected $streamPointer;

  /**
   * 构造方法。
   */
  public function __construct() {
    $_SESSION['file_example']['.isadir.txt'] = TRUE;
  }

  /**
   * Implements setUri().
   */
  public function setUri($uri) {
    $this->uri = $uri;
  }

  /**
   * Implements getUri().
   */
  public function getUri() {
    return $this->uri;
  }

  /**
   * Implements getTarget().
   *
   * "target" 是 URI 中 scheme 的右边的部分，比如 session://example/test.txt中，
   * target 就是 'example/test.txt'。
   */
  public function getTarget($uri = NULL) {
    if (!isset($uri)) {
      $uri = $this->uri;
    }

    list($scheme, $target) = explode('://', $uri, 2);

    // 从 session:// 模式中删除错误的斜线。
    return trim($target, '\/');
  }

  /**
   * Implements getMimeType().
   */
  public static function getMimeType($uri, $mapping = NULL) {
    if (!isset($mapping)) {

      // file.mimetypes.inc 中定义的缺省文件映射比较大，我们只在必要的时候载入。
      include_once DRUPAL_ROOT . '/includes/file.mimetypes.inc';
      $mapping = file_mimetype_mapping();
    }

    $extension = '';
    $file_parts = explode('.', basename($uri));

    // 移除文件名的第一段：主文件名不会是扩展名。
    array_shift($file_parts);

    // 遍历文件名的每个部分来进行匹配：
    // 例如 my.awesome.image.jpeg，我们会尝试：
    // - jpeg
    // - image.jpeg
    // - awesome.image.jpeg
    while ($additional_part = array_pop($file_parts)) {
      $extension = drupal_strtolower($additional_part . ($extension ? '.' . $extension : ''));
      if (isset($mapping['extensions'][$extension])) {
        return $mapping['mimetypes'][$mapping['extensions'][$extension]];
      }
    }

    return 'application/octet-stream';
  }

  /**
   * Implements getDirectoryPath().
   *
   * 这里没有目录，所以返回空。
   */
  public function getDirectoryPath() {
    return '';
  }

  /**
   * Overrides getExternalUrl().
   *
   * 我们设置了一个辅助函数和菜单入口，通过 HTTP 进行访问；一般情况下应该提供一些其他途径
   * 来访问。
   */
  public function getExternalUrl() {
    $path = $this->getLocalPath();
    $url = url('examples/file_example/access_session/' . $path, array('absolute' => TRUE));
    return $url;
  }

  /**
   * 这个例子中没有权限，所以直接返回 TRUE。
   */
  public function chmod($mode) {
    return TRUE;
  }

  /**
   * Implements realpath().
   */
  public function realpath() {
    return 'session://' . $this->getLocalPath();
  }

  /**
   * 返回本地路径
   *
   * 这里我们只是把 "file" 作为键保存在 $_SESSION 变量中，所以只需要创建一个虚拟的路径
   * 这个路径实际上是 $_SESSION 变量的一个键。'session://one/two/three.txt' 会转换为
   * $_SESSION['file_example']['one']['two']['three.txt'] 。
   *
   * @param string $uri
   *   URI，可选，用于移动或者重命名。
   */
  protected function getLocalPath($uri = NULL) {
    if (!isset($uri)) {
      $uri = $this->uri;
    }

    $path  = str_replace('session://', '', $uri);
    $path = trim($path, '/');
    return $path;
  }

  /**
   * 打开一个 Stream，为 fopen()，file_get_contents()，file_put_contents() 工作。
   *
   * @param string $uri
   *   URI，其中包含了需要打开的文件描述。
   * @param string $mode
   *   打开方式 （"r", "wb" 之类）。
   * @param int $options
   *   STREAM_USE_PATH 和 STREAM_REPORT_ERRORS 组成的胃掩码。
   * @param string &$opened_path
   *   实际打开的路径。
   *
   * @return bool
   *   如果成功打开，则返回 TRUE（这里总是 TRUE ）。
   *
   * @see http://php.net/manual/en/streamwrapper.stream-open.php
   */
  public function stream_open($uri, $mode, $options, &$opened_path) {
    $this->uri = $uri;

    // 我们把 $session_content 作为一个到 $_SESSION 变量的值的引用。
    // 例如 /example/test.txt 就是
    // $_SESSION['file_example']['example']['test.txt']的引用。
    $this->sessionContent = &$this->uri_to_session_key($uri);

    // 复位指针。
    $this->streamPointer = 0;
    return TRUE;
  }

  /**
   * 返回一个到正确的 $_SESSION 键的引用。
   *
   * @param string $uri
   *    uri，例如 session://something
   * @param bool $create
   *   如果是 TRUE 就创建这个键。
   *
   * @return array|bool
   *   一个到键路径最后一节的引用。如果该路径找不到对应的键路径，并且 $create 参数为
   *   FALSE, 则返回 FALSE。
   */
  protected function &uri_to_session_key($uri, $create = TRUE) {
    // 因为 uri_to_session_key() 方法返回的是一个引用，我们必须设置一个失败标记变量。
    $fail = FALSE;
    $path = $this->getLocalPath($uri);
    if (strlen($path) == 0 )
      $path_components = array();
    else
      $path_components = explode('/', $path);
    // 设置一个到 session:// '根目录' 的引用。
    $var = &$_SESSION['file_example'];
    // 处理仅是一个 session:// 的情况
    if (count($path_components) < 1) {
      return $var;
    }

    // 除非说明不允许新建。
    // 否则会遍历路径的各个部分，并在 $_SESSION 中创建相应的键。
    foreach ($path_components as $component) {
      if ($create || isset($var[$component])) {
        $var = &$var[$component];
      }
      else {
        // 不能存在该路径对应的键，也不允许创建。
        return $fail;
      }
    }
    return $var;
  }

  /**
   * flock() 的支持
   *
   * $_SESSION 变量没有锁定机制，所以返回 TRUE。
   *
   * @param int $operation
   *   下面的某一个值：
   *   - LOCK_SH 申请一个共享锁（读）。
   *   - LOCK_EX 申请一个独占锁（写）。
   *   - LOCK_UN 释放一个锁。
   *   - LOCK_NB 以非阻塞方式执行（Windows 不支持）。
   *
   * @return bool
   *   目前只会返回 TRUE （不支持锁操作）
   *
   * @see http://php.net/manual/en/streamwrapper.stream-lock.php
   */
  public function stream_lock($operation) {
    return TRUE;
  }

  /**
   * fread()，file_get_contents() 等的支持
   *
   * @param int $count
   *   要读入的最大字节数。
   *
   * @return string
   *   如果错误则返回 FALSE。
   *
   * @see http://php.net/manual/en/streamwrapper.stream-read.php
   */
  public function stream_read($count) {
    if (is_string($this->sessionContent)) {
      $remaining_chars = drupal_strlen($this->sessionContent) - $this->streamPointer;
      $number_to_read = min($count, $remaining_chars);
      if ($remaining_chars > 0) {
        $buffer = drupal_substr($this->sessionContent, $this->streamPointer, $number_to_read);
        $this->streamPointer += $number_to_read;
        return $buffer;
      }
    }
    return FALSE;
  }

  /**
   * 对 fwrite(), file_put_contents() 等的支持.
   *
   * @param string $data
   *   要写入的内容。
   *
   * @return int
   *   要写入的字节数。
   *
   * @see http://php.net/manual/en/streamwrapper.stream-write.php
   */
  public function stream_write($data) {
    // 因为这些数据要写入会话变量，因此这里需要对内容进行一些简单的清理。
    $data = check_plain($data);
    $this->sessionContent = substr_replace($this->sessionContent, $data, $this->streamPointer);
    $this->streamPointer += drupal_strlen($data);
    return drupal_strlen($data);
  }

  /**
   * feof() 的支持。
   *
   * @return bool
   *   如果到达尾端，则返回 TRUE。
   *
   * @see http://php.net/manual/en/streamwrapper.stream-eof.php
   */
  public function stream_eof() {
    return FALSE;
  }

  /**
   * fseek() 支持.
   *
   * @param int $offset
   *   前往的字节偏移量。
   * @param int $whence
   *   SEEK_SET， SEEK_CUR， 或者 SEEK_END。
   *
   * @return bool
   *   成功则返回 TRUE。
   *
   * @see http://php.net/manual/en/streamwrapper.stream-seek.php
   */
  public function stream_seek($offset, $whence) {
    if (drupal_strlen($this->sessionContent) >= $offset) {
      $this->streamPointer = $offset;
      return TRUE;
    }
    return FALSE;
  }

  /**
   * fflush() 支持。
   *
   * @return bool

   *   如果数据成功存储（或者没有需要存储的数据），则返回 TRUE。因为没有提供缓存支持，
   *   这里只会返回 TRUE。
   *
   * @see http://php.net/manual/en/streamwrapper.stream-flush.php
   */
  public function stream_flush() {
    return TRUE;
  }

  /**
   * ftell() 支持。
   *
   * @return int
   *   从头部开始计算的当前的字节偏移量。
   *
   * @see http://php.net/manual/en/streamwrapper.stream-tell.php
   */
  public function stream_tell() {
    return $this->streamPointer;
  }

  /**
   * fstat() 支持。
   *
   * @return array
   *   一个包含文件状态的数组，如果出错则返回 FALSE，可参考 fstat() 函数来了解数组描述。
   *
   * @see http://php.net/manual/en/streamwrapper.stream-stat.php
   */
  public function stream_stat() {
    return array(
      'size' => drupal_strlen($this->sessionContent),
    );
  }

  /**
   * fclose() 支持。
   *
   * @return bool
   *   如果成功关闭则返回 TRUE。
   *
   * @see http://php.net/manual/en/streamwrapper.stream-close.php
   */
  public function stream_close() {
    $this->streamPointer = 0;
    // 清空引用。
    unset($this->sessionContent);
    return TRUE;
  }

  /**
   * unlink() 支持。
   *
   * @param string $uri
   *   一个包含要删除 URI 的字符串。
   *
   * @return bool
   *   如果成功删除则返回 TRUE。
   *
   * @see http://php.net/manual/en/streamwrapper.unlink.php
   */
  public function unlink($uri) {
    $path = $this->getLocalPath($uri);
    $path_components = preg_split('/\//', $path);
    $fail = FALSE;
    $unset = '$_SESSION[\'file_example\']';
    foreach ($path_components as $component) {
      $unset .= '[\'' . $component . '\']';
    }
    // TODO: 有没有更好的从数组删除的方法？
    // drupal_array_get_nested_value() 没用，是因为他返回的是引用，unset 引用只是
    // 删除引用本身。
    eval("unset($unset);");
    return TRUE;
  }

  /**
   * rename() 支持。
   *
   * @param string $from_uri
   *   要重命名的 URI
   * @param string $to_uri
   *   新的 URI
   *
   * @return bool
   *   如果成功重命名，则返回 TRUE 。
   *
   * @see http://php.net/manual/en/streamwrapper.rename.php
   */
  public function rename($from_uri, $to_uri) {
    $from_key = &$this->uri_to_session_key($from_uri);
    $to_key = &$this->uri_to_session_key($to_uri);
    if (is_dir($to_key) || is_file($to_key)) {
      return FALSE;
    }
    $to_key = $from_key;
    unset($from_key);
    return TRUE;
  }

  /**
   * 从给定路径中获取目录名
   *
   * @param string $uri
   *   URI.
   *
   * @return string
   *   目录名称的字符串。
   *
   * @see drupal_dirname()
   */
  public function dirname($uri = NULL) {
    list($scheme, $target) = explode('://', $uri, 2);
    $target  = $this->getTarget($uri);
    if (strpos($target, '/')) {
      $dirname = preg_replace('@/[^/]*$@', '', $target);
    }
    else {
      $dirname = '';
    }
    return $scheme . '://' . $dirname;
  }

  /**
   * mkdir() 支持。
   *
   * @param string $uri
   *   一个代表将要创建目录的 URI.
   * @param int $mode
   *   权限标志 - 参看 mkdir()。
   * @param int $options
   *   位掩码，STREAM_REPORT_ERRORS 以及 STREAM_MKDIR_RECURSIVE.
   *
   * @return bool
   *   如果成功建立目录则返回 TRUE。
   *
   * @see http://php.net/manual/en/streamwrapper.mkdir.php
   */
  public function mkdir($uri, $mode, $options) {
    // 如果已经存在，则无法完成新建。
    if (is_dir($uri) || is_file($uri)) {
      return FALSE;
    }

    // 在 $_SESSION 创建这个键；
    $this->uri_to_session_key($uri, TRUE);

    // 在目录中放置一个魔术文件，以跟空文件进行区分。
    $marker_uri = $uri . '/.isadir.txt';
    $this->uri_to_session_key($marker_uri, TRUE);
    return TRUE;
  }

  /**
   * rmdir() 支持。
   *
   * @param string $uri
   *   包含待删除目录的 URI。
   * @param int $options
   *   位掩码，STREAM_REPORT_ERRORS。
   *
   * @return bool
   *   如果成功删除，则返回 TRUE。
   *
   * @see http://php.net/manual/en/streamwrapper.rmdir.php
   */
  public function rmdir($uri, $options) {
    $path = $this->getLocalPath($uri);
    $path_components = preg_split('/\//', $path);
    $fail = FALSE;
    $unset = '$_SESSION[\'file_example\']';
    foreach ($path_components as $component) {
      $unset .= '[\'' . $component . '\']';
    }
    // TODO: 我真的不喜欢 eval
    debug($unset, 'array element to be unset');
    eval("unset($unset);");

    return TRUE;
  }

  /**
   * stat() 支持。
   *
   * 这个函数在使用 Unix 方式的工作中非常重要。这个例子中的 stat 数组很古怪，但是 mode
   * 是非常重要的。他用来告诉 PHP，是否存在某个文件或者目录，他有什么权限。跟 PHP 通常
   * 的做法不太一样，这里所有的返回都是位掩码的方式。
   *
   * @param string $uri
   *   包含我们要获取信息的目标 URI
   * @param int $flags
   *  位掩码，包括 STREAM_URL_STAT_LINK 和 STREAM_URL_STAT_QUIET.
   *
   * @return array|bool
   *   文件状态数组，如果出错则返回 FALSE - 在 fstat() 中可以看到这个数组的描述。
   *
   *
   * @see http://php.net/manual/en/streamwrapper.url-stat.php
   */
  public function url_stat($uri, $flags) {

    // 根据 URI 获取 $_SESSION 键的引用。
    $key = $this->uri_to_session_key($uri, FALSE);


    // 缺省设置失败。
    $return = FALSE;
    $mode = 0;

    // 如果是一个数组，那么我们当他是个目录，根就是目录。
    if (is_array($key) && array_key_exists('.isadir.txt', $key)) {
      // S_IFDIR 代表这是个目录。
      $mode = 0040000;
    }
    elseif ($key !== FALSE) {
      // S_IFREG 说明这是个文件。
      $mode = 0100000;
    }


    if ($mode) {
      $size = 0;
      if ($mode == 0100000) {
        $size = drupal_strlen($key);
      }

      // 这里没什么写保护，所以是全部可写。
      $mode |= 0777;
      $return = array(
        'dev' => 0,
        'ino' => 0,
        'mode' => $mode,
        'nlink' => 0,
        'uid' => 0,
        'gid' => 0,
        'rdev' => 0,
        'size' => $size,
        'atime' => 0,
        'mtime' => 0,
        'ctime' => 0,
        'blksize' => 0,
        'blocks' => 0,
      );
    }
    return $return;
  }

  /**
   * opendir() 的支持。
   *
   * @param string $uri
   *   代表目标目录的 URI。
   * @param int $options
   *   是否使用 safe_mode (0x04).
   *
   * @return bool
   *   成功则返回 TRUE。
   *
   * @see http://php.net/manual/en/streamwrapper.dir-opendir.php
   */
  public function dir_opendir($uri, $options) {
    $var = &$this->uri_to_session_key($uri, FALSE);
    if ($var === FALSE || !array_key_exists('.isadir.txt', $var)) {
      return FALSE;
    }

    // 我们获取一个键名列表，然后用 array_flip 做反转，来移除 .isadir.txt。
    // 然后就能够获取到列表了。
    $this->directoryKeys = array_flip(array_keys($var));
    unset($this->directoryKeys['.isadir.txt']);
    $this->directoryKeys = array_keys($this->directoryKeys);
    $this->directoryPointer = 0;
    return TRUE;
  }

  /**
   * readdir() 支持。
   *
   * @return string|bool
   *   下一个文件名, 如果没有下一个，则返回 FALSE。
   *
   * @see http://php.net/manual/en/streamwrapper.dir-readdir.php
   */
  public function dir_readdir() {
    if ($this->directoryPointer < count($this->directoryKeys)) {
      $next = $this->directoryKeys[$this->directoryPointer];
      $this->directoryPointer++;
      return $next;
    }
    return FALSE;
  }

  /**
   * rewinddir() 支持。
   *
   * @return bool
   *   成功则返回 TRUE。
   *
   * @see http://php.net/manual/en/streamwrapper.dir-rewinddir.php
   */
  public function dir_rewinddir() {
    $this->directoryPointer = 0;
  }

  /**
   * closedir() 支持。
   *
   * @return bool
   *   成功则返回 TRUE。
   *
   * @see http://php.net/manual/en/streamwrapper.dir-closedir.php
   */
  public function dir_closedir() {
    $this->directoryPointer = 0;
    unset($this->directoryKeys);
    return TRUE;
  }
}
