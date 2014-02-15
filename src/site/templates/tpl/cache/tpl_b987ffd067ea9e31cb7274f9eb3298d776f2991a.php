<!DOCTYPE html>
<html lang="<?php echo $__tpl_vars__LANG_SHORT; ?>">
	<head>
		<meta charset="utf-8" />
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
		<title><?php echo $__tpl_vars__PAGE_TITLE; ?> &bull; Bisounours Party</title>
		<!--[if IE]>
		<script src="http://html5shiv.googlecode.com/svn/trunk/html5.js?1282601430"></script>
		<![endif]-->
		<link rel="stylesheet" type="text/css" href="<?php echo $__tpl_vars__BASE_URL; ?>/templates/css/html5reset.css?1282601430" />
		<link rel="stylesheet" type="text/css" href="<?php echo $__tpl_vars__BASE_URL; ?>/templates/css/style.css?1282601430" />
	</head>
	<body>
		<header></header>
		<nav id="speedbarre">
			<ul>
				<li><a href="<?php echo $__tpl_vars__BASE_URL; ?>/"><?php echo $__tpl_vars__LANG_SPEEDBARRE_HOME; ?></a></li>
                                <li><a href="<?php echo $__tpl_vars__BASE_URL; ?>/<?php echo $__tpl_vars__LANG_URL_FORUM; ?>/"><?php echo $__tpl_vars__LANG_SPEEDBARRE_FORUM; ?></a></li>
				<li><a href="<?php echo $__tpl_vars__BASE_URL; ?>/<?php echo $__tpl_vars__LANG_URL_BLOG; ?>/"><?php echo $__tpl_vars__LANG_SPEEDBARRE_BLOG; ?></a></li>
				<li><a href="<?php echo $__tpl_vars__BASE_URL; ?>/<?php echo $__tpl_vars__LANG_URL_DEVBLOG; ?>/"><?php echo $__tpl_vars__LANG_SPEEDBARRE_DEVBLOG; ?></a></li>
				<li><a href="<?php echo $__tpl_vars__BASE_URL; ?>/<?php echo $__tpl_vars__LANG_URL_MEDIAS; ?>/"><?php echo $__tpl_vars__LANG_SPEEDBARRE_MEDIAS; ?></a></li>
				<li><a href="<?php echo $__tpl_vars__BASE_URL; ?>/<?php echo $__tpl_vars__LANG_URL_TEAM; ?>/"><?php echo $__tpl_vars__LANG_SPEEDBARRE_TEAM; ?></a></li>
				<li><a href="<?php echo $__tpl_vars__BASE_URL; ?>/<?php echo $__tpl_vars__LANG_URL_ABOUT; ?>/"><?php echo $__tpl_vars__LANG_SPEEDBARRE_ABOUT; ?></a></li>
				<li><a href="<?php echo $__tpl_vars__BASE_URL; ?>/<?php echo $__tpl_vars__LANG_URL_DOWNLOADS; ?>/"><?php echo $__tpl_vars__LANG_SPEEDBARRE_DOWNLOADS; ?></a></li>
			</ul>
		</nav>
		<nav id="menu">
			<?php if ($__tpl_vars__SECTION == 'blog' OR $__tpl_vars__SECTION == 'devblog' OR $__tpl_vars__SECTION == 'adminBlog' OR $__tpl_vars__SECTION == 'adminDevBlog') : ?>
			<nav>
				<h1><?php echo $__tpl_vars__LANG_MENU_CATEGORIES; ?></h1>
				<ul>
				<?php if ($tpl->block('menu_categories', null)) : $__tpl_d3215db19e1487c67786cce91e28ecd59e79de83 = &$tpl->block('menu_categories', null); $__tpl_block_stack[] = 'menu_categories'; foreach ($__tpl_d3215db19e1487c67786cce91e28ecd59e79de83 as &$__tplBlock['menu_categories']){ ?>
					<?php if ($__tpl_vars__SECTION == 'blog') : ?>
					<li><a href="<?php echo $__tpl_vars__BASE_URL; ?>/blog/<?php echo $__tpl_vars__LANG_URL_BLOG_CATEGORY; ?>/<?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_ID']; ?>-<?php echo Talus_TPL_Filters::niceurl($__tplBlock['menu_categories']['MENU_CATEGORY_NAME']); ?>"><?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_NAME']; ?> (<?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_ARTICLES']; ?>)</a></li>
					<?php elseif ($__tpl_vars__SECTION == 'devblog') : ?>
					<li><a href="<?php echo $__tpl_vars__BASE_URL; ?>/devblog/<?php echo $__tpl_vars__LANG_URL_DEVBLOG_CATEGORY; ?>/<?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_ID']; ?>-<?php echo Talus_TPL_Filters::niceurl($__tplBlock['menu_categories']['MENU_CATEGORY_NAME']); ?>"><?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_NAME']; ?> (<?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_ARTICLES']; ?>)</a></li>
                                    	<?php elseif ($__tpl_vars__SECTION == 'adminBlog') : ?>
					<li><a href="/admin/blog/editCategorie/<?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_ID']; ?>"><img src="/templates/img/modify.png" /></a>   <a href="/admin/blog/deleteCategorie/<?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_ID']; ?>"><img src="/templates/img/delete.png" /></a><a href="/blog/categorie/<?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_ID']; ?>-<?php echo Talus_TPL_Filters::niceurl($__tplBlock['menu_categories']['MENU_CATEGORY_NAME']); ?>"><?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_NAME']; ?> (<?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_ARTICLES']; ?>)</a></li>
                                   	<?php elseif ($__tpl_vars__SECTION == 'adminDevBlog') : ?>
					<li><a href="/admin/devblog/editCategorie/<?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_ID']; ?>"><img src="/templates/img/modify.png" /></a>   <a href="/admin/devblog/deleteCategorie/<?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_ID']; ?>"><img src="/templates/img/delete.png" /></a><a href="/devblog/categorie/<?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_ID']; ?>-<?php echo Talus_TPL_Filters::niceurl($__tplBlock['menu_categories']['MENU_CATEGORY_NAME']); ?>"><?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_NAME']; ?> (<?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_ARTICLES']; ?>)</a></li>
					<?php endif; ?>
				<?php } else : if (true) { $__tpl_block_stack[] = '*foo*'; ?>
					<li><?php echo $__tpl_vars__LANG_MENU_CATEGORIES_NOTFOUND; ?></li>
				<?php } unset($__tplBlock[array_pop($__tpl_block_stack)]); endif; ?>
                                <?php if ($__tpl_vars__SECTION == 'adminBlog') : ?>
					<li><a href="/admin/blog/newCategorie/"><?php echo $__tpl_vars__LANG_ADMIN_CAT_NEW; ?></a></li>
                                <?php elseif ($__tpl_vars__SECTION == 'adminDevBlog') : ?>
                                        <li><a href="/admin/devblog/newCategorie/"><?php echo $__tpl_vars__LANG_ADMIN_CAT_NEW; ?></a></li>
				<?php endif; ?>
				</ul>
			</nav>
			<?php endif; ?>

			<nav>
				<h1><?php echo $__tpl_vars__LANG_MENU_LINKS; ?></h1>
				<ul>
					<li><a href="http://www.moddb.com/mods/bisounours-party">Page ModDB</a></li>
					<li><a href="http://www.geek-gaming.fr/site/">Geek-Gaming</a></li>
				</ul>
			</nav>
                    <nav>
				<h1><?php echo $__tpl_vars__LANG_MENU_USER; ?></h1>
				<ul>
                                    <?php if ($__tpl_vars__ISCONNECTED == 1) : ?>
                                        <li><?php echo $__tpl_vars__LANG_MENU_CONNECTEDAS; ?> <a href="http://www.bisounoursparty.com/forum/profile.php?id=<?php echo $__tpl_vars__USER_ID; ?>"><?php echo $__tpl_vars__USER_NICK; ?></a></li>
                                        <li><a href="http://www.bisounoursparty.com/forum/login.php?action=out&id=<?php echo $__tpl_vars__USER_ID; ?>&csrf_token=<?php echo $__tpl_vars__USER_OUTTOKEN; ?>"><?php echo $__tpl_vars__LANG_MENU_DISCONNECT; ?></a></li>
                                        <?php if ($__tpl_vars__USER_CLOUDLINKED == 0) : ?>
                                            <li><a href="http://www.bisounoursparty.com/link/"><?php echo $__tpl_vars__LANG_MENU_LINKSTEAMACCOUNT; ?></a></li>
                                        <?php endif; ?>
                                        <?php if ($__tpl_vars__USER_ADMIN == 1) : ?>
                                            <li><a href="http://www.bisounoursparty.com/admin/"><?php echo $__tpl_vars__LANG_MENU_ADMIN; ?></a></li>
                                        <?php endif; ?>
                                    <?php else : ?>
                                        <p><?php echo $__tpl_vars__LANG_MENU_NOTCONNECTED; ?></p>
                                        <li><a href="http://www.bisounoursparty.com/forum/login.php"><?php echo $__tpl_vars__LANG_MENU_CONNECT; ?></a></li>
                                    <?php endif; ?>
				</ul>
			</nav>
		</nav>
		<div id="main">