<?php $tpl->includeTpl('header.html', false, Talus_TPL::INCLUDE_TPL); ?>
			<?php if ($tpl->block('articles', null)) : $__tpl_87d52b1d5917827920c2e2173cacbab228f4369c = &$tpl->block('articles', null); $__tpl_block_stack[] = 'articles'; foreach ($__tpl_87d52b1d5917827920c2e2173cacbab228f4369c as &$__tplBlock['articles']){ ?>
			<article>
				<header>
					<h2><a href="/<?php echo $__tpl_vars__LANG_URL_DEVBLOG; ?>/<?php echo $__tpl_vars__LANG_URL_ARTICLE; ?>/<?php echo $__tplBlock['articles']['ARTICLE_ID']; ?>-<?php echo Talus_TPL_Filters::niceurl($__tplBlock['articles']['ARTICLE_TITLE']); ?>"><?php echo $__tplBlock['articles']['ARTICLE_TITLE']; ?></a></h2>
				</header>
				<article>
					<time>
						<div class="news_day"><?php echo $__tplBlock['articles']['ARTICLE_DATE_DAY']; ?></div>
						<div class="news_month"><?php echo $__tplBlock['articles']['ARTICLE_DATE_MONTH']; ?></div>
						<div class="news_year"><?php echo $__tplBlock['articles']['ARTICLE_DATE_YEAR']; ?></div>
						<div class="news_time"><?php echo $__tplBlock['articles']['ARTICLE_DATE_HOUR']; ?>h<?php echo $__tplBlock['articles']['ARTICLE_DATE_MINUTE']; ?></div>
					</time>
					<?php echo Talus_TPL_Filters::markdown($__tplBlock['articles']['ARTICLE_CONTENT']); ?>
				</article>
				<footer>
					<?php echo $__tpl_vars__LANG_BLOG_ARTICLE_AUTHOR; ?><a href="http://www.bisounoursparty.com/forum/profile.php?id=<?php echo $__tplBlock['articles']['ARTICLE_AUTHOR_ID']; ?>" class="author"><?php echo $__tplBlock['articles']['ARTICLE_AUTHOR_NICK']; ?></a><?php echo $__tpl_vars__LANG_BLOG_ARTICLE_CATEGORY; ?><a href="/<?php echo $__tpl_vars__LANG_URL_DEVBLOG; ?>/<?php echo $__tpl_vars__LANG_URL_BLOG_CATEGORY; ?>/<?php echo $__tplBlock['articles']['ARTICLE_CATEGORY_ID']; ?>-<?php echo Talus_TPL_Filters::niceurl($__tplBlock['articles']['ARTICLE_CATEGORY_NAME']); ?>"><?php echo $__tplBlock['articles']['ARTICLE_CATEGORY_NAME']; ?></a>
				</footer>
			</article>
			<?php } else : if (true) { $__tpl_block_stack[] = '*foo*'; ?>
                            <p><?php echo $__tpl_vars__LANG_BLOG_NO_ARTICLES; ?></p>
			<?php } unset($__tplBlock[array_pop($__tpl_block_stack)]); endif; ?>
             <?php if ($__tpl_vars__COMMENTS == 0) : ?>
	             <?php if ($__tpl_vars__BLOG_NUMBERPAGES != 1) : ?>
        		    <p>
			            <?php if ($tpl->block('pages', null)) : $__tpl_b080e1df7d6c0c12a3cb6e0cef6d6ef74ee4ebf3 = &$tpl->block('pages', null); $__tpl_block_stack[] = 'pages'; foreach ($__tpl_b080e1df7d6c0c12a3cb6e0cef6d6ef74ee4ebf3 as &$__tplBlock['pages']){ ?>
							<a href="/<?php echo $__tpl_vars__LANG_URL_DEVBLOG; ?>/<?php echo $__tpl_vars__LANG_URL_ARTICLE; ?>/page-<?php echo $__tplBlock['pages']['NUM']; ?>"><?php echo $__tplBlock['pages']['NUM']; ?></a>
        	    		<?php } unset($__tplBlock[array_pop($__tpl_block_stack)]); endif; ?>
    		        </p>
	            <?php endif; ?>
            <?php endif; ?>
                        <?php if ($__tpl_vars__COMMENTS == 1) : ?>
                            <script language="javascript"> 
function toggle() {
                    var ele = document.getElementById("toggleText");
                    if(ele.style.display == "block") {
                            ele.style.display = "none";
                    }
                    else {
                            ele.style.display = "block";
                    }
                  }
function cleartextarea(id){
if (clickedIt == false){
id.value="";
clickedIt=true;
}
} </script>
                        <article>
				<header>
					<h2><a href="/<?php echo $__tpl_vars__LANG_URL_DEVBLOG; ?>/<?php echo $__tpl_vars__LANG_URL_ARTICLE; ?>/<?php echo $__tplBlock['articles']['ARTICLE_ID']; ?>-<?php echo Talus_TPL_Filters::niceurl($__tplBlock['articles']['ARTICLE_TITLE']); ?>"><?php echo $__tpl_vars__LANG_BLOG_COMMENTS; ?></a></h2>
				</header>
				<article>
			<?php if ($tpl->block('comments', null)) : $__tpl_e85a4457387ef20a3e8fed5a20865f1431537619 = &$tpl->block('comments', null); $__tpl_block_stack[] = 'comments'; foreach ($__tpl_e85a4457387ef20a3e8fed5a20865f1431537619 as &$__tplBlock['comments']){ ?>
                            <a href="http://www.bisounoursparty.com/forum/profile.php?id=<?php echo $__tplBlock['comments']['COMMENT_AUTHOR_ID']; ?>">
                            <?php if ($__tplBlock['comments']['COMMENT_AUTHOR_GROUPID'] == 1) : ?>
					<h4 class="medium red awesome"><?php echo $__tplBlock['comments']['COMMENT_AUTHOR_NICK']; ?></h4>
                            <?php elseif ($__tplBlock['comments']['COMMENT_AUTHOR_GROUPID'] == 5) : ?>
                                        <h4 class="medium red awesome"><?php echo $__tplBlock['comments']['COMMENT_AUTHOR_NICK']; ?></h4>
                            <?php elseif ($__tplBlock['comments']['COMMENT_AUTHOR_GROUPID'] == 6) : ?>
                                        <h4 class="medium orange awesome"><?php echo $__tplBlock['comments']['COMMENT_AUTHOR_NICK']; ?></h4>
                            <?php elseif ($__tplBlock['comments']['COMMENT_AUTHOR_GROUPID'] == 7) : ?>
                                        <h4 class="medium orange awesome"><?php echo $__tplBlock['comments']['COMMENT_AUTHOR_NICK']; ?></h4>
                            <?php elseif ($__tplBlock['comments']['COMMENT_AUTHOR_GROUPID'] == 8) : ?>
                                        <h4 class="medium green awesome"><?php echo $__tplBlock['comments']['COMMENT_AUTHOR_NICK']; ?></h4>
                            <?php else : ?>
                                        <h4 class="medium blue awesome"><?php echo $__tplBlock['comments']['COMMENT_AUTHOR_NICK']; ?></h4>
                            <?php endif; ?>
                                </a>
                            <?php if ($__tpl_vars__USER_CAN_MODERATE == 1) : ?>
                                <a href="/<?php echo $__tpl_vars__LANG_URL_DEVBLOG; ?>/DelComment/<?php echo $__tplBlock['comments']['COMMENT_ID']; ?>-<?php echo $__tplBlock['articles']['ARTICLE_ID']; ?>"><img src="/templates/img/delete.png" /></a>
                            <?php elseif ($__tpl_vars__USER_ID == $__tplBlock['comments']['COMMENT_AUTHOR_ID']) : ?>
                                <a href="/<?php echo $__tpl_vars__LANG_URL_DEVBLOG; ?>/DelComment/<?php echo $__tplBlock['comments']['COMMENT_ID']; ?>-<?php echo $__tplBlock['articles']['ARTICLE_ID']; ?>"><img src="/templates/img/delete.png" /></a>
                            <?php endif; ?>
                                        <blockquote style="padding: 8px;"><?php echo Talus_TPL_Filters::markdown($__tplBlock['comments']['COMMENT_CONTENT']); ?></blockquote>
			<?php } else : if (true) { $__tpl_block_stack[] = '*foo*'; ?>
                            <p><?php echo $__tpl_vars__LANG_BLOG_NO_COMMENT; ?></p>
			<?php } unset($__tplBlock[array_pop($__tpl_block_stack)]); endif; ?>
				</article>
				<footer>
                                    <a href="javascript:toggle();"><?php echo $__tpl_vars__LANG_BLOG_COMMENT_ADD; ?></a><!--<a href="/<?php echo $__tpl_vars__LANG_URL_MEMBER; ?>/<?php echo $__tplBlock['comments']['COMMENT_AUTHOR_ID']; ?>-<?php echo Talus_TPL_Filters::niceurl($__tplBlock['comments']['COMMENT_AUTHOR_NICK']); ?>" class="author"><?php echo $__tplBlock['comments']['COMMENT_AUTHOR_NICK']; ?></a><?php echo $__tpl_vars__LANG_BLOG_COMMENT_CATEGORY; ?><a href="/<?php echo $__tpl_vars__LANG_URL_DEVBLOG; ?>/<?php echo $__tpl_vars__LANG_URL_ARTICLE; ?>/<?php echo $__tplBlock['comments']['ARTICLE_ID']; ?>-<?php echo Talus_TPL_Filters::niceurl($__tplBlock['comments']['ARTICLE_TITLE']); ?>"><?php echo $__tplBlock['comments']['ARTICLE_TITLE']; ?></a>-->
				</footer>
                        </article>
                        
                            <article id="toggleText" style="display: none">
                                    <header>
                                            <h2><?php echo $__tpl_vars__LANG_BLOG_COMMENT_ADD; ?></h2>
                                    </header>
                                    <article>
                                        <?php if ($__tpl_vars__ISCONNECTED == 1) : ?>
                                            <form action="/<?php echo $__tpl_vars__LANG_URL_DEVBLOG; ?>/AddComment/<?php echo $__tplBlock['articles']['ARTICLE_ID']; ?>-<?php echo Talus_TPL_Filters::niceurl($__tplBlock['articles']['ARTICLE_TITLE']); ?>" method="post">
                                                <p><?php echo Talus_TPL_Filters::markdown($__tpl_vars__LANG_BLOG_USINGMD); ?></p>
                                                <textarea name="content" rows="8" cols="80" onFocus="this.value=''; this.onfocus=null;" >
                                                </textarea>
                                            <input type="submit" value="<?php echo $__tpl_vars__LANG_SEND; ?>" />    
                                            </form>
                                        <?php else : ?>
                                            <p><?php echo Talus_TPL_Filters::markdown($__tpl_vars__LANG_BLOG_NEEDTOCONNECT); ?> abc</p>
                                        <?php endif; ?>
                                    </article>
                                    <footer>
                                    </footer>
                                
                            </article>
                        
                        <?php endif; ?>
<?php $tpl->includeTpl('footer.html', false, Talus_TPL::INCLUDE_TPL); ?>