<?php require_once('libs/markdown.php'); ?>
<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8" />
		<title>Test du design &bull; Bisounours Party</title>
		<!--[if IE]>
		<script src="http://html5shiv.googlecode.com/svn/trunk/html5.js?1282601430"></script>
		<![endif]-->
		<link rel="stylesheet" type="text/css" href="./templates/css/html5reset.css?1282601430" />
		<link rel="stylesheet" type="text/css" href="./templates/css/style.css?1282601430" />
	</head>
	<body>
		<header></header>
		<nav id="speedbarre">
			<ul>
				<li><a href="./blog/">Accueil</a></li>
				<li><a href="./devblog/">Dev-blog</a></li>
				<li><a href="./media/">Medias</a></li>
				<li><a href="./equipe/">Équipe</a></li>
				<li><a href="./a-propos/">Le mod</a></li>
				<li><a href="./telechargement/">Téléchargement</a></li>
			</ul>
		</nav>
		<nav id="menu">
			<nav>
				<h1>Catégories</h1>
				<ul>
					<li><a href="./blog/categorie/mod">Le mod</a></li>
					<li><a href="./blog/categorie/site">Le site</a></li>
					<li><a href="./blog/categorie/catlol">Catégorie</a></li>
				</ul>
			</nav>

			<nav>
				<h1>Liens externes</h1>
				<ul>
					<li><a href="http://www.moddb.com/mods/bisounours-party">Page ModDB</a></li>
					<li><a href="http://www.geek-gaming.fr/site/">Geek-Gaming</a></li>
				</ul>
			</nav>
		</nav>
		<div id="main">
			<article>
				<header>
					<h2>My first news</h2>
				</header>
				<article>
					<time>
						<div class="news_day">25</div>
						<div class="news_month">01</div>
						<div class="news_year">1987</div>
						<div class="news_time">10h07</div>
					</time>
					<?php echo Markdown(file_get_contents('lang/fr/static/guide/guide.md')); ?>
				</article>
				<footer>
					Publié par <a href="/membre/Sargeros" class="author">Sargeros</a> dans <a href="/blog/categorie/Test">Test</a>, <a href="/blog/categorie/Categorie">Categorie</a>
				</footer>
			</article>
		</div>
		<footer>
			<p>Designer original : Xandar - Adaptation : Sargeros - Copyright 2010 Bisounours Party - Toute copie partielle ou totale est interdite</p>
		</footer>
	</body>
</html>