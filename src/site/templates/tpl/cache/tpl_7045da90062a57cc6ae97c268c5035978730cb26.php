<?php $tpl->includeTpl('header.html', false, Talus_TPL::INCLUDE_TPL); ?>
			<article>
				<header>
					<h2><a href="http://www.bisounoursparty.com/admin/"><?php echo $__tpl_vars__PAGE_TITLE; ?></a> > <a href="http://www.bisounoursparty.com/admin/blog/">Blog</a> > <?php echo $__tpl_vars__LANG_ADMIN_EDIT; ?></h2>
				</header>
                                <article>
                                            <form action="/admin/blog/updateArticle/<?php echo $__tpl_vars__ARTICLE_ID; ?>" method="post">
                                            <p><?php echo Talus_TPL_Filters::markdown($__tpl_vars__LANG_BLOG_USINGMD); ?></p>
                                            <input type="text" name="title" id="title" size="60" value="<?php echo $__tpl_vars__ARTICLE_TITLE; ?>"/>
                                            <textarea name="content" rows="16" cols="80"><?php echo $__tpl_vars__ARTICLE_CONTENT; ?></textarea>
                                                   <select name="categorie" id="categorie">
                                                       <?php if ($tpl->block('menu_categories', null)) : $__tpl_af848ea17ef48c20cd342f6fa819d6232d8466fb = &$tpl->block('menu_categories', null); $__tpl_block_stack[] = 'menu_categories'; foreach ($__tpl_af848ea17ef48c20cd342f6fa819d6232d8466fb as &$__tplBlock['menu_categories']){ ?>
                                                           <?php if ($__tpl_vars__ARTICLE_CATEGORY == $__tplBlock['menu_categories']['MENU_CATEGORY_ID'] ) : ?>
                                                                <option value="<?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_ID']; ?>" selected="selected"><?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_NAME']; ?></option>
                                                            <?php else : ?>
                                                                <option value="<?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_ID']; ?>"><?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_NAME']; ?></option>
                                                           <?php endif; ?>
                                                       <?php } unset($__tplBlock[array_pop($__tpl_block_stack)]); endif; ?>
                                                   </select>
                                                   <?php if ($__tpl_vars__ARTICLE_LANG == 'fr') : ?>
                                                <input type="radio" name="lang" value="fr" id="fr" checked="checked" /> <label for="fr">Français</label>
                                                <input type="radio" name="lang" value="en" id="en" /> <label for="en">English</label>
                                                <?php else : ?>
                                                <input type="radio" name="lang" value="fr" id="fr" /> <label for="fr">Français</label>
                                                <input type="radio" name="lang" value="en" id="en" checked="checked" /> <label for="en">English</label>
                                                   <?php endif; ?>
                                        <input type="submit" value="<?php echo $__tpl_vars__LANG_SEND; ?>" />    
                                        </form>
				</article>
                            	<footer>
				</footer>
			</article>
<?php $tpl->includeTpl('footer.html', false, Talus_TPL::INCLUDE_TPL); ?>