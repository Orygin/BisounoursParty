<?php
        require_once('start.php');
        
	include('./lang/' . USER_LANG . '/images.php');
        
        $dir = "templates/img/medias/";
        $cssdir = SITE_BASE_URL . "/templates/css/boxifier.css";
        $TPL->set('HEAD_TAG','<style>@import url(\''. $cssdir .'\');</style>');
        
        if (is_dir($dir)) {
            if ($dh = opendir($dir)) {
                while (($file = readdir($dh)) !== false) {
                        if (($file == '.')||($file == '..'))
                        {
                        }
                        elseif (is_dir($tdir.'/'.$file))
                        {
                                filesInDir($tdir.'/'.$file);
                        }
                        else
                        {
                            $TPL->block('images', array('CODE' => "<a href='../" . $dir . '' . $file . "' rel='boxifier[imgs]'><img src='../" . $dir . '' . $file . "' width='167' height='167' /></a>"));
                        }
                        //echo "fichier : $file : type : " . filetype($dir . $file) . "<br />";
                }
                closedir($dh);
            }
        }

	$TPL->set('PAGE_TITLE', $lang['LANG_MEDIA_PAGE_TITLE']);

	$TPL->set($lang);

	$TPL->parse('images.html');
