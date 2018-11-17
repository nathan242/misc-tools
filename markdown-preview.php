<!doctype html>
  <head>
    <title>Markdown Preview</title>
  </head>
  <body>

  <div>
    <form method='post'>
      <textarea name="markdown" rows=20 placeholder="Enter markdown text" style="width: 100%;"><?php if (isset($_POST['markdown'])) { echo $_POST['markdown']; } ?></textarea>
      <p><input type="submit" value="SUBMIT" /></p>
    </form>
  </div>

  <hr>

  <div>
<?php
    if (isset($_POST['markdown'])) {
        $d = array(
            0 => array('pipe', 'r'),
            1 => array('pipe', 'w'),
            2 => array('pipe', 'w')
        );

        $pipes = '';

        $proc = proc_open('markdown', $d, $pipes, '/tmp', array());

        fwrite($pipes[0], $_POST['markdown']);
        fclose($pipes[0]);

        echo stream_get_contents($pipes[1]);
        fclose($pipes[1]);

        fclose($pipes[2]);
        proc_close($proc);
    }
?>
 </div>

  </body>
</html>
