<?php
        require_once('start.php');
        
	include('./lang/' . USER_LANG . '/admin.php');

	$TPL->set($lang);
        
        if(!IsAdmin($pun_user))
        {
            $TPL->parse('errors/RefusedAccess.html');
            exit();
        }
        
        if($_GET['page'] == 'blog')
        {
            if($_GET['action'] == 'edit')
            {
                $categories = $SQL->query('SELECT id, name_' . USER_LANG . ' AS name, articles FROM ' . MYSQL_PREFIX . 'blog_categories ORDER BY name ASC');
                while($data = $categories->fetch()) {
                            $TPL->block('menu_categories', array(
                            'MENU_CATEGORY_ID' => $data['id'],
                            'MENU_CATEGORY_NAME' => $data['name'],
                            'MENU_CATEGORY_ARTICLES' => $data['articles']
		));
                }
                $article_id = intval($_GET['id']);
                $articles = $SQL->query('SELECT     a.id AS articleId,
                                                    a.categorie AS articleCat,
                                                    a.title AS articleTitle,
                                                    a.content AS articleContent,
                                                    a.comments AS articleComments,
                                                    u.id AS authorId,
                                                    u.username AS authorNick,
                                                    a.lang AS articleLang,
                                                    c.name_' . USER_LANG . ' AS categorieName
                       			FROM ' . MYSQL_PREFIX . 'blog_articles a
					INNER JOIN bpforum_users u
						ON a.author = u.id
                                        INNER JOIN ' . MYSQL_PREFIX . 'blog_categories c
						ON a.categorie = c.id
                                        WHERE a.id = '.$article_id.'
					ORDER BY a.date DESC');//WHERE a.lang = "' . USER_LANG . '" AND a.status = 1
                
		 while($article = $articles->fetch()) {
                    $TPL->set(array('ARTICLE_ID' => $article['articleId'],
                                    'ARTICLE_CATEGORY' => $article['articleCat'],
                                    'ARTICLE_TITLE' => $article['articleTitle'],
                                    'ARTICLE_CONTENT' => $article['articleContent'],
                                    'ARTICLE_AUTHOR_ID' => $article['authorId'],
                                    'ARTICLE_AUTHOR_NICK' => $article['authorNick'],
                                    'ARTICLE_CATEGORY_NAME' => $article['categorieName'],
                                    'ARTICLE_CATEGORY_ID' => $article['articleCat'],
                                    'ARTICLE_LANG' => $article['articleLang'],
                                    ));
                            }
                $TPL->set('SECTION', 'adminBlog');
                $TPL->parse('admin/blog_edit.html');   
            }
            elseif($_GET['action'] == 'editcat')
            {                
                $id = intval($_GET['id']);

                $categories = $SQL->prepare('SELECT id, name_fr, name_en FROM bp_blog_categories WHERE id = :id');
                
                $categ = $categories->execute(array('id'=>$id));
                while($data = $categories->fetch()) {
                            $TPL->set(array(
                            'CATEGORY_ID' => $data['id'],
                            'CATEGORY_NAME_FR' => $data['name_fr'],
                            'CATEGORY_NAME_EN' => $data['name_en']
		));
                }
                
                $TPL->set('SECTION', 'adminBlogCat');
                $TPL->set('UPDATE', '1');
                $TPL->parse('admin/blog_cat_edit.html');  
            }
            elseif($_GET['action'] == 'newcat')
            {
                $TPL->set('SECTION', 'adminBlogCat');
                $TPL->set('UPDATE', '0');
                $TPL->parse('admin/blog_cat_edit.html');  
            }
            else
            {
              if($_GET['action'] == 'add')
                {
                    $content = utf8_encode(htmlspecialchars ($_POST['content'], ENT_QUOTES));
                    $title = utf8_encode(htmlspecialchars ($_POST['title'], ENT_QUOTES));
                    $cat = $_POST['categorie'];
                    $lang = $_POST['lang'];
                    
                    if(!empty($content))
                    {
                        $db->query("INSERT INTO " . MYSQL_PREFIX . "blog_articles (id, author, categorie, date, title, lang, status, content) 
                            VALUES (NULL, '" . $pun_user['id'] . "', '".$cat."', NOW(), '".$title."', '".$lang."', 1, '" . $content . "')");                
                    }
                }
                elseif($_GET['action'] == 'del')
                {
                    $delid = intval($_GET['id']);
                    $req = $SQL->prepare("DELETE FROM bp_blog_articles WHERE id = :id");
                    $req->execute(array('id' => $delid));
                }
                elseif($_GET['action'] == 'update')
                {
                    $content = htmlspecialchars ($_POST['content'], ENT_QUOTES);
                    $title = htmlspecialchars ($_POST['title'], ENT_QUOTES);
                    $cat = $_POST['categorie'];
                    $lang = $_POST['lang'];
                    $id = intval($_GET['id']);
                    
                    if(!empty($content) && !empty($title))
                    {
                        $req = $SQL->prepare("UPDATE " . MYSQL_PREFIX . "blog_articles
                            SET categorie=:cat, title=:title, lang=:lang, content=:content WHERE id=:id");
                        $req->execute(array('cat' =>$cat,
                                            'title' => $title,
                                            'lang'=>$lang,
                                            'content'=>$content,
                                            'id'=>$id));
                    }
                }
                elseif($_GET['action'] == 'updatecat')
                {
                    $name_fr = utf8_encode(htmlspecialchars ($_POST['name_fr'], ENT_QUOTES));
                    $name_en = utf8_encode(htmlspecialchars ($_POST['name_en'], ENT_QUOTES));
                    $id = intval($_GET['id']);
                    
                    if(!empty($name_fr) AND !empty($name_en))
                    {
                        $req = $SQL->prepare("UPDATE " . MYSQL_PREFIX . "blog_categories
                            SET name_fr=:fr, name_en=:en WHERE id=:id");
                        $req->execute(array('en' => $name_en,
                                            'fr' => $name_fr,
                                            'id'=> $id));
                    }
                }
                elseif($_GET['action'] == 'delcat')
                {
                    $delid = intval($_GET['id']);
                    $req = $SQL->prepare("DELETE FROM bp_blog_categories WHERE id = :id");
                    $req->execute(array('id' => $delid));
                }
                elseif($_GET['action'] == 'addcat')
                {
                    $name_fr = utf8_encode(htmlspecialchars ($_POST['name_fr'], ENT_QUOTES));
                    $name_en = utf8_encode(htmlspecialchars ($_POST['name_en'], ENT_QUOTES));
                    
                    if(!empty($name_fr) AND !empty($name_en))
                    {
                        $req = $SQL->prepare("INSERT INTO " . MYSQL_PREFIX . "blog_categories (id, name_fr, name_en) 
                            VALUES (NULL, :fr, :en)");
                        $req->execute(array('fr' => $name_fr, 'en'=>$name_en));
                    }
                }
                
             $categories = $SQL->query('SELECT id, name_' . USER_LANG . ' AS name, articles FROM ' . MYSQL_PREFIX . 'blog_categories ORDER BY name ASC');

            while($data = $categories->fetch()) {
            $articles_name[$data['id']] = $data['name'];
            		$TPL->block('menu_categories', array(
			'MENU_CATEGORY_ID' => $data['id'],
			'MENU_CATEGORY_NAME' => $data['name'],
			'MENU_CATEGORY_ARTICLES' => $data['articles']
		));
                }

                $articles = $SQL->query('SELECT     a.id AS articleId,
                                                    a.categorie AS articleCat,
                                                    DAY(a.date) AS articleDateDay,
                                                    MONTH(a.date) AS articleDateMonth,
                                                    YEAR(a.date) AS articleDateYear,
                                                    HOUR(a.date) AS articleDateHour,
                                                    MINUTE(a.date) AS articleDateMinute,
                                                    a.title AS articleTitle,
                                                    a.content AS articleContent,
                                                    a.comments AS articleComments,
                                                    u.id AS authorId,
                                                    u.username AS authorNick,
                                                    a.lang AS articleLang
                       			FROM ' . MYSQL_PREFIX . 'blog_articles a
					INNER JOIN bpforum_users u
						ON a.author = u.id
					ORDER BY a.date DESC');//WHERE a.lang = "' . USER_LANG . '" AND a.status = 1
                
		 while($article = $articles->fetch()) {
                     			$TPL->block('articlelist', array(
				'ARTICLE_ID' => $article['articleId'],
				'ARTICLE_CATEGORY' => $article['articleCat'],
				'ARTICLE_TITLE' => $article['articleTitle'],
				'ARTICLE_AUTHOR_ID' => $article['authorId'],
				'ARTICLE_AUTHOR_NICK' => $article['authorNick'],
				'ARTICLE_CATEGORY_NAME' => $articles_name[$article['articleCat']],
                                'ARTICLE_CATEGORY_ID' => $article['articleCat'],
                                'ARTICLE_LANG' => $article['articleLang'],
			));
                 }
                $TPL->set('SECTION', 'adminBlog');
                $TPL->parse('admin/blog_list.html');
            }
        }
        elseif($_GET['page'] == 'devblog')
        {
            if($_GET['action'] == 'edit')
            {
                $categories = $SQL->query('SELECT id, name_' . USER_LANG . ' AS name, articles FROM ' . MYSQL_PREFIX . 'devblog_categories ORDER BY name ASC');
                while($data = $categories->fetch()) {
                            $TPL->block('menu_categories', array(
                            'MENU_CATEGORY_ID' => $data['id'],
                            'MENU_CATEGORY_NAME' => $data['name'],
                            'MENU_CATEGORY_ARTICLES' => $data['articles']
		));
                }
                $article_id = intval($_GET['id']);
                $articles = $SQL->query('SELECT     a.id AS articleId,
                                                    a.categorie AS articleCat,
                                                    a.title AS articleTitle,
                                                    a.content AS articleContent,
                                                    a.comments AS articleComments,
                                                    u.id AS authorId,
                                                    u.username AS authorNick,
                                                    a.lang AS articleLang,
                                                    c.name_' . USER_LANG . ' AS categorieName
                       			FROM ' . MYSQL_PREFIX . 'devblog_articles a
					INNER JOIN bpforum_users u
						ON a.author = u.id
                                        INNER JOIN ' . MYSQL_PREFIX . 'devblog_categories c
						ON a.categorie = c.id
                                        WHERE a.id = '.$article_id.'
					ORDER BY a.date DESC');//WHERE a.lang = "' . USER_LANG . '" AND a.status = 1
                
		 while($article = $articles->fetch()) {
                    $TPL->set(array('ARTICLE_ID' => $article['articleId'],
                                    'ARTICLE_CATEGORY' => $article['articleCat'],
                                    'ARTICLE_TITLE' => $article['articleTitle'],
                                    'ARTICLE_CONTENT' => $article['articleContent'],
                                    'ARTICLE_AUTHOR_ID' => $article['authorId'],
                                    'ARTICLE_AUTHOR_NICK' => $article['authorNick'],
                                    'ARTICLE_CATEGORY_NAME' => $article['categorieName'],
                                    'ARTICLE_CATEGORY_ID' => $article['articleCat'],
                                    'ARTICLE_LANG' => $article['articleLang'],
                                    ));
                            }
                $TPL->set('SECTION', 'adminDevBlog');
                $TPL->parse('admin/devblog_edit.html');   
            }
            elseif($_GET['action'] == 'editcat')
            {                
                $id = intval($_GET['id']);

                $categories = $SQL->prepare('SELECT id, name_fr, name_en FROM bp_devblog_categories WHERE id = :id');
                
                $categ = $categories->execute(array('id'=>$id));
                while($data = $categories->fetch()) {
                            $TPL->set(array(
                            'CATEGORY_ID' => $data['id'],
                            'CATEGORY_NAME_FR' => $data['name_fr'],
                            'CATEGORY_NAME_EN' => $data['name_en']
		));
                }
                
                $TPL->set('SECTION', 'adminDevBlogCat');
                $TPL->set('UPDATE', '1');
                $TPL->parse('admin/devblog_cat_edit.html');  
            }
            elseif($_GET['action'] == 'newcat')
            {
                $TPL->set('SECTION', 'adminDevBlogCat');
                $TPL->set('UPDATE', '0');
                $TPL->parse('admin/devblog_cat_edit.html');  
            }
            else
            {
              if($_GET['action'] == 'add')
                {
                    $content = utf8_encode(htmlspecialchars ($_POST['content'], ENT_QUOTES));
                    $title = utf8_encode(htmlspecialchars ($_POST['title'], ENT_QUOTES));
                    $cat = $_POST['categorie'];
                    $lang = $_POST['lang'];
                    
                    if(!empty($content))
                    {
                        $db->query("INSERT INTO " . MYSQL_PREFIX . "devblog_articles (id, author, categorie, date, title, lang, status, content) 
                            VALUES (NULL, '" . $pun_user['id'] . "', '".$cat."', NOW(), '".$title."', '".$lang."', 1, '" . $content . "')");                
                    }
                }
                elseif($_GET['action'] == 'addcat')
                {
                    $name_fr = utf8_encode(htmlspecialchars ($_POST['name_fr'], ENT_QUOTES));
                    $name_en = utf8_encode(htmlspecialchars ($_POST['name_en'], ENT_QUOTES));
                    
                    if(!empty($name_fr) AND !empty($name_en))
                    {
                        $req = $SQL->prepare("INSERT INTO " . MYSQL_PREFIX . "devblog_categories (id, name_fr, name_en) 
                            VALUES (NULL, :fr, :en)");
                        $req->execute(array('fr' => $name_fr, 'en'=>$name_en));
                    }
                }
                elseif($_GET['action'] == 'del')
                {
                    $delid = intval($_GET['id']);
                    $req = $SQL->prepare("DELETE FROM bp_devblog_articles WHERE id = :id");
                    $req->execute(array('id' => $delid));
                }
                elseif($_GET['action'] == 'delcat')
                {
                    $delid = intval($_GET['id']);
                    $req = $SQL->prepare("DELETE FROM bp_devblog_categories WHERE id = :id");
                    $req->execute(array('id' => $delid));
                }
                elseif($_GET['action'] == 'update')
                {
                    $content = htmlspecialchars ($_POST['content'], ENT_QUOTES);
                    $title = htmlspecialchars ($_POST['title'], ENT_QUOTES);
                    $cat = $_POST['categorie'];
                    $lang = $_POST['lang'];
                    $id = intval($_GET['id']);
                    
                    if(!empty($content) && !empty($title))
                    {
                        $req = $SQL->prepare("UPDATE " . MYSQL_PREFIX . "devblog_articles
                            SET categorie=:cat, title=:title, lang=:lang, content=:content WHERE id=:id");
                        $req->execute(array('cat' =>$cat,
                                            'title' => $title,
                                            'lang'=>$lang,
                                            'content'=>$content,
                                            'id'=>$id));
                    }
                }
                elseif($_GET['action'] == 'updatecat')
                {
                    $name_fr = utf8_encode(htmlspecialchars ($_POST['name_fr'], ENT_QUOTES));
                    $name_en = utf8_encode(htmlspecialchars ($_POST['name_en'], ENT_QUOTES));
                    $id = intval($_GET['id']);
                    
                    if(!empty($name_fr) AND !empty($name_en))
                    {
                        $req = $SQL->prepare("UPDATE " . MYSQL_PREFIX . "devblog_categories
                            SET name_fr=:fr, name_en=:en WHERE id=:id");
                        $req->execute(array('en' => $name_en,
                                            'fr' => $name_fr,
                                            'id'=> $id));
                    }
                }
             $categories = $SQL->query('SELECT id, name_' . USER_LANG . ' AS name, articles FROM ' . MYSQL_PREFIX . 'devblog_categories ORDER BY name ASC');

            while($data = $categories->fetch()) {
            $articles_name[$data['id']] = $data['name'];
            		$TPL->block('menu_categories', array(
			'MENU_CATEGORY_ID' => $data['id'],
			'MENU_CATEGORY_NAME' => $data['name'],
			'MENU_CATEGORY_ARTICLES' => $data['articles']
		));
                }

                $articles = $SQL->query('SELECT     a.id AS articleId,
                                                    a.categorie AS articleCat,
                                                    DAY(a.date) AS articleDateDay,
                                                    MONTH(a.date) AS articleDateMonth,
                                                    YEAR(a.date) AS articleDateYear,
                                                    HOUR(a.date) AS articleDateHour,
                                                    MINUTE(a.date) AS articleDateMinute,
                                                    a.title AS articleTitle,
                                                    a.content AS articleContent,
                                                    a.comments AS articleComments,
                                                    u.id AS authorId,
                                                    u.username AS authorNick,
                                                    a.lang AS articleLang
                       			FROM ' . MYSQL_PREFIX . 'devblog_articles a
					INNER JOIN bpforum_users u
						ON a.author = u.id
					ORDER BY a.date DESC');//WHERE a.lang = "' . USER_LANG . '" AND a.status = 1
                
		 while($article = $articles->fetch()) {
                     			$TPL->block('articlelist', array(
				'ARTICLE_ID' => $article['articleId'],
				'ARTICLE_CATEGORY' => $article['articleCat'],
				'ARTICLE_TITLE' => $article['articleTitle'],
				'ARTICLE_AUTHOR_ID' => $article['authorId'],
				'ARTICLE_AUTHOR_NICK' => $article['authorNick'],
				'ARTICLE_CATEGORY_NAME' => $articles_name[$article['articleCat']],
                                'ARTICLE_CATEGORY_ID' => $article['articleCat'],
                                'ARTICLE_LANG' => $article['articleLang'],
			));
                 }
                $TPL->set('SECTION', 'adminDevBlog');
                $TPL->parse('admin/devblog_list.html');
            }
        }
        else if($_GET['page'] == 'team')
        {
            if($_GET['action']== 'add')
            {
                $id = $_POST['user_id'];
                $poste = $_POST['poste'];
                $req = $SQL->prepare('INSERT INTO '.MYSQL_PREFIX.'team (user_id, poste) VALUES(:id, :poste)');
                $req->execute(array('poste' => $poste, 'id' =>$id));
            }
            else if($_GET['action'] == 'edit')
            {
                $id = intval($_GET['id']);
                $poste = $_POST['poste'];
                $req = $SQL->prepare('UPDATE '.MYSQL_PREFIX.'team SET poste=:poste WHERE user_id=:id');
                $req->execute(array('poste' => $poste, 'id'=>$id));
            }
            else if($_GET['action'] == 'del')
            {
                $id = intval($_GET['id']);
                $req = $SQL->prepare('DELETE FROM '.MYSQL_PREFIX.'team WHERE user_id=:id');
                $req->execute(array('id'=>$id));
            }
            
         $req = $SQL->prepare('SELECT    t.user_id AS USER_ID,
                                        t.poste AS USER_POSTE,
                                        u.username AS USER_NICK
                                        FROM ' . MYSQL_PREFIX . 'team t
                                        INNER JOIN bpforum_users u
                                            ON t.user_id = u.id');
            $req->execute();
            while($user = $req->fetch())
            {
              $TPL->block('users', array('USER_ID' => $user['USER_ID'],
                                            'USER_POSTE' => $user['USER_POSTE'],
                                            'USER_NICK' => $user['USER_NICK']));
            }
            $TPL->parse('admin/team.html');
        }
        else
        {
            $TPL->set('LANG_ADMIN_WELCOME', file_get_contents('./lang/' . USER_LANG . '/static/admin.md'));

            $TPL->parse('admin/admin.html');
        }