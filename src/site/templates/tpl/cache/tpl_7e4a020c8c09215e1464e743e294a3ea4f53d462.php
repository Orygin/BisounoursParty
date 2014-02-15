			<?php if ($tpl->block('comments', null)) : $__tpl_48a62a395ba23204dea0834da92c3ed10b2ff42b = &$tpl->block('comments', null); $__tpl_block_stack[] = 'comments'; foreach ($__tpl_48a62a395ba23204dea0834da92c3ed10b2ff42b as &$__tplBlock['comments']){ ?>
			<article>
				<header>
					<h2><a href="/<?php echo $__tpl_vars__LANG_URL_DEVBLOG; ?>/<?php echo $__tpl_vars__LANG_URL_ARTICLE; ?>/<?php echo $__tplBlock['articles']['ARTICLE_ID']; ?>-<?php echo Talus_TPL_Filters::niceurl($__tplBlock['articles']['ARTICLE_TITLE']); ?>"><?php echo $__tplBlock['articles']['ARTICLE_TITLE']; ?></a></h2>
				</header>
				<article>
					<!--<time>
						<div class="news_day"><?php echo $__tplBlock['comments']['ARTICLE_DATE_DAY']; ?></div>
						<div class="news_month"><?php echo $__tplBlock['comments']['ARTICLE_DATE_MONTH']; ?></div>
						<div class="news_year"><?php echo $__tplBlock['comments']['ARTICLE_DATE_YEAR']; ?></div>
						<div class="news_time"><?php echo $__tplBlock['comments']['ARTICLE_DATE_HOUR']; ?>h<?php echo $__tplBlock['comments']['ARTICLE_DATE_MINUTE']; ?></div>
					</time>-->
					<?php echo Talus_TPL_Filters::markdown($__tplBlock['comments']['COMMENT_CONTENT']); ?>
				</article>
				<footer>
					<?php echo $__tpl_vars__LANG_BLOG_COMMENT_AUTHOR; ?><a href="/<?php echo $__tpl_vars__LANG_URL_MEMBER; ?>/<?php echo $__tplBlock['comments']['COMMENT_AUTHOR_ID']; ?>-<?php echo Talus_TPL_Filters::niceurl($__tplBlock['comments']['COMMENT_AUTHOR_NICK']); ?>" class="author"><?php echo $__tplBlock['comments']['COMMENTS_AUTHOR_NICK']; ?></a><?php echo $__tpl_vars__LANG_BLOG_COMMENTS_CATEGORY; ?><a href="/<?php echo $__tpl_vars__LANG_URL_DEVBLOG; ?>/<?php echo $__tpl_vars__LANG_URL_BLOG_CATEGORY; ?>/<?php echo $__tplBlock['articles']['ARTICLE_CATEGORY_ID']; ?>-<?php echo Talus_TPL_Filters::niceurl($__tplBlock['articles']['ARTICLE_CATEGORY_NAME']); ?>"><?php echo $__tplBlock['articles']['ARTICLE_TITLE']; ?></a>
				</footer>
			</article>
			<?php } else : if (true) { $__tpl_block_stack[] = '*foo*'; ?>