<?php
	require_once('start.php');

	$TPL->set('SECTION', 'blog');

	include('./lang/' . USER_LANG . '/blog.php');


	$categories = $SQL->query('SELECT id, name_' . USER_LANG . ' AS name, articles FROM ' . MYSQL_PREFIX . 'blog_categories ORDER BY name ASC');

	while($data = $categories->fetch()) {
	$articles_name[$data['id']] = $data['name'];
		$TPL->block('menu_categories', array(
			'MENU_CATEGORY_ID' => $data['id'],
			'MENU_CATEGORY_NAME' => $data['name'],
			'MENU_CATEGORY_ARTICLES' => $data['articles']
		));
	}

	$catId = intval($_GET['cat_id']);
        $artId = intval($_GET['art_id']);

        if($_GET['Add'] == 1)
        {
            $content = utf8_encode(htmlspecialchars ($_POST['content'], ENT_QUOTES));
            if(!empty($content) AND !empty($artId) AND is_int($artId))
            {
                $db->query("INSERT INTO `orygin`.`bp_blog_comments` (`id`, `author`, `article`, `date`, `content`) 
                    VALUES (NULL, '" . $pun_user['id'] . "', '" . $artId . "', NOW(), '" . $content . "')");                
            }
        }

        $delid = intval($_GET['del']);
        if(!empty($delid))
        {
            if($pun_user['group_id'] == 1 || $pun_user['group_id'] == 5 || $pun_user['group_id'] == 2)
            {
                $req = $SQL->prepare("DELETE FROM bp_blog_comments WHERE id = :id");
                $req->execute(array('id' => $delid));
            }
            else{
                $userid = intval($pun_user['id']);
                $req = $SQL->prepare("DELETE FROM bp_blog_comments WHERE id=':id' AND author=':auth'");
                $req->execute(array('id' => $delid, 'auth' => $userid));
            }
        }

        if(!empty($artId)) {
		$articles = $SQL->query('SELECT
										 a.id AS articleId,
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
										 u.username AS authorNick
									FROM ' . MYSQL_PREFIX . 'blog_articles a
									INNER JOIN bpforum_users u
										ON a.author = u.id
									WHERE a.id = "' . $artId . '" AND a.status = 1
									ORDER BY a.date DESC
									LIMIT 5');
		 while($article = $articles->fetch()) {
			$TPL->block('articles', array(
				'ARTICLE_ID' => $article['articleId'],
				'ARTICLE_CATEGORY' => $article['articleCat'],
				'ARTICLE_DATE_DAY' => $article['articleDateDay'],
				'ARTICLE_DATE_MONTH' => $article['articleDateMonth'],
				'ARTICLE_DATE_YEAR' => $article['articleDateYear'],
				'ARTICLE_DATE_HOUR' => $article['articleDateHour'],
				'ARTICLE_DATE_MINUTE' => $article['articleDateMinute'],
				'ARTICLE_TITLE' => $article['articleTitle'],
				'ARTICLE_CONTENT' => $article['articleContent'],
				'ARTICLE_COMMENTS' => $article['articleComments'],
				'ARTICLE_AUTHOR_ID' => $article['authorId'],
				'ARTICLE_AUTHOR_NICK' => $article['authorNick'],
				'ARTICLE_CATEGORY_NAME' => $articles_name[$article['articleCat']],
                                'ARTICLE_CATEGORY_ID' => $article['articleCat'],
			));
                        $nomarticle = $article['articleTitle'];
                        $idarticle = $article['articleId'];
		 }
                 
                 $TPL->Set('COMMENTS','1');
		$comments = $SQL->query('SELECT          a.id AS commentId,
							 DAY(a.date) AS commentDateDay,
							 MONTH(a.date) AS commentDateMonth,
							 YEAR(a.date) AS commentDateYear,
							 HOUR(a.date) AS commentDateHour,
							 MINUTE(a.date) AS commentDateMinute,
							 a.content AS commentContent,
							 u.id AS authorId,
							 u.username AS authorNick,
                                                         u.group_id as authorGroupID
						FROM ' . MYSQL_PREFIX . 'blog_comments a
						INNER JOIN bpforum_users u
							ON a.author = u.id
						WHERE a.article = ' . $artId . '
						ORDER BY a.date');
		 while($comment = $comments->fetch()) {
			$TPL->block('comments', array(
				'COMMENT_ID' => $comment['commentId'],
				'COMMENT_CATEGORY' => $comment['articleCat'],
				'COMMENT_DATE_DAY' => $comment['commentDateDay'],
				'COMMENT_DATE_MONTH' => $comment['commentDateMonth'],
				'COMMENT_DATE_YEAR' => $comment['commentDateYear'],
				'COMMENT_DATE_HOUR' => $comment['commentDateHour'],
				'COMMENT_DATE_MINUTE' => $comment['commentDateMinute'],
				'ARTICLE_TITLE' => $nomarticle,
                                'ARTICLE_ID' => $idarticle,
				'COMMENT_CONTENT' => $comment['commentContent'],
				'COMMENT_COMMENTS' => $comment['commentComments'],
				'COMMENT_AUTHOR_ID' => $comment['authorId'],
				'COMMENT_AUTHOR_NICK' => $comment['authorNick'],
                                'COMMENT_AUTHOR_GROUPID' => $comment['authorGroupID'],
			));
                 }
                 $TPL->set('LANG_BLOG_NEEDTOCONNECT', file_get_contents('./lang/' . USER_LANG . '/static/ConnectToComment.md'));
	}
        else
        {
	
	if(!empty($catId)) {
		$return = $SQL->query('SELECT COUNT(*) as num FROM ' . MYSQL_PREFIX . 'blog_articles WHERE lang = "' . USER_LANG . '" AND status = 1 AND categorie = ' . $catId . '');
        $count = $return->fetch();

		if(!empty($_GET['page']) && $count['num'] > 5)
		{
			$page = intval($_GET['page']);
			$limit = "LIMIT ".(($page-1)*5).", 5";
		}
		else
			$limit = "LIMIT 0,5";
		
		$num_pages = ceil($count['num'] / 5);
		$TPL->set('BLOG_NUMBERPAGES', $num_pages);
		for($i = 0; $i < $num_pages; $i++)
		{
				$TPL->block('pages', 'NUM', $i+1);
		}
			

			/*echo 'SELECT
										 a.id AS articleId,
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
										 u.username AS authorNick
									FROM ' . MYSQL_PREFIX . 'blog_articles a
									INNER JOIN bpforum_users u
										ON a.author = u.id
									WHERE a.categorie = ' . $catId . ' AND a.lang = "' . USER_LANG . '" AND a.status = 1
									ORDER BY a.date DESC
									' . $limit . '';*/
		
		 $articles = $SQL->query('SELECT
										 a.id AS articleId,
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
										 u.username AS authorNick
									FROM ' . MYSQL_PREFIX . 'blog_articles a
									INNER JOIN bpforum_users u
										ON a.author = u.id
									WHERE a.categorie = ' . $catId . ' AND a.lang = "' . USER_LANG . '" AND a.status = 1
									ORDER BY a.date DESC
									' . $limit . '');
									
		 while($article = $articles->fetch()) {
			$TPL->block('articles', array(
				'ARTICLE_ID' => $article['articleId'],
				'ARTICLE_CATEGORY' => $article['articleCat'],
				'ARTICLE_DATE_DAY' => $article['articleDateDay'],
				'ARTICLE_DATE_MONTH' => $article['articleDateMonth'],
				'ARTICLE_DATE_YEAR' => $article['articleDateYear'],
				'ARTICLE_DATE_HOUR' => $article['articleDateHour'],
				'ARTICLE_DATE_MINUTE' => $article['articleDateMinute'],
				'ARTICLE_TITLE' => $article['articleTitle'],
				'ARTICLE_CONTENT' => $article['articleContent'],
				'ARTICLE_COMMENTS' => $article['articleComments'],
				'ARTICLE_AUTHOR_ID' => $article['authorId'],
				'ARTICLE_AUTHOR_NICK' => $article['authorNick'],
				'ARTICLE_CATEGORY_NAME' => $articles_name[$article['articleCat']],
                'ARTICLE_CATEGORY_ID' => $article['articleCat'],
			));
		 }
	}
	else {
		$return = $SQL->query('SELECT COUNT(*) as num FROM ' . MYSQL_PREFIX . 'blog_articles WHERE lang = "' . USER_LANG . '" AND status = 1');
        $count = $return->fetch();

		if(!empty($_GET['page']) && $count['num'] > 5)
		{
			$page = intval($_GET['page']);
			$limit = "LIMIT ".(($page-1)*5).", 5";
		}
		else
			$limit = "LIMIT 0,5";
		
		$num_pages = ceil($count['num'] / 5);
		$TPL->set('BLOG_NUMBERPAGES', $num_pages);
		for($i = 0; $i < $num_pages; $i++)
		{
				$TPL->block('pages', 'NUM', $i+1);
		}
			
		$articles = $SQL->query('SELECT
										 a.id AS articleId,
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
										 u.username AS authorNick
									FROM ' . MYSQL_PREFIX . 'blog_articles a
									INNER JOIN bpforum_users u
										ON a.author = u.id
									WHERE a.lang = "' . USER_LANG . '" AND a.status = 1
									ORDER BY a.date DESC
									' . $limit . '');
		 while($article = $articles->fetch()) {
			$TPL->block('articles', array(
				'ARTICLE_ID' => $article['articleId'],
				'ARTICLE_CATEGORY' => $article['articleCat'],
				'ARTICLE_DATE_DAY' => $article['articleDateDay'],
				'ARTICLE_DATE_MONTH' => $article['articleDateMonth'],
				'ARTICLE_DATE_YEAR' => $article['articleDateYear'],
				'ARTICLE_DATE_HOUR' => $article['articleDateHour'],
				'ARTICLE_DATE_MINUTE' => $article['articleDateMinute'],
				'ARTICLE_TITLE' => $article['articleTitle'],
				'ARTICLE_CONTENT' => $article['articleContent'],
				'ARTICLE_COMMENTS' => $article['articleComments'],
				'ARTICLE_AUTHOR_ID' => $article['authorId'],
				'ARTICLE_AUTHOR_NICK' => $article['authorNick'],
				'ARTICLE_CATEGORY_NAME' => $articles_name[$article['articleCat']],
                                'ARTICLE_CATEGORY_ID' => $article['articleCat'],
			));
		 }
	}
        }
	$TPL->set($lang);
	$TPL->parse('blog/blog_index.html');