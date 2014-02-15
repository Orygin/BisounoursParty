<?php $tpl->includeTpl('header.html', false, Talus_TPL::INCLUDE_TPL); ?>
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
					<h2><a href="http://www.bisounoursparty.com/admin/"><?php echo $__tpl_vars__PAGE_TITLE; ?></a> > <a href="http://www.bisounoursparty.com/admin/blog/">Blog</a> > <?php echo $__tpl_vars__LANG_ADMIN_LIST; ?></h2>
				</header>
                                <article><ul>
					<?php if ($tpl->block('articlelist', null)) : $__tpl_3f744eda0ce77a0d8d70d8bb09657ba36cc2408c = &$tpl->block('articlelist', null); $__tpl_block_stack[] = 'articlelist'; foreach ($__tpl_3f744eda0ce77a0d8d70d8bb09657ba36cc2408c as &$__tplBlock['articlelist']){ ?>
                                            <li><img src="/templates/img/<?php echo $__tplBlock['articlelist']['ARTICLE_LANG']; ?>.png" />    <a href="/blog/article/<?php echo $__tplBlock['articlelist']['ARTICLE_ID']; ?>"><?php echo $__tplBlock['articlelist']['ARTICLE_TITLE']; ?></a><?php echo $__tpl_vars__LANG_ADMIN_BY; ?><a href="http://www.bisounoursparty.com/forum/profile.php?id=<?php echo $__tplBlock['articlelist']['ARTICLE_AUTHOR_ID']; ?>"><?php echo $__tplBlock['articlelist']['ARTICLE_AUTHOR_NICK']; ?></a><?php echo $__tpl_vars__LANG_ADMIN_IN; ?><a href="/admin/blog/categorie/<?php echo $__tplBlock['articlelist']['ARTICLE_CATEGORY']; ?>"><?php echo $__tplBlock['articlelist']['ARTICLE_CATEGORY_NAME']; ?></a>    <a href="/admin/blog/editArticle/<?php echo $__tplBlock['articlelist']['ARTICLE_ID']; ?>"><img src="/templates/img/modify.png" /></a>   <a href="/admin/blog/deleteArticle/<?php echo $__tplBlock['articlelist']['ARTICLE_ID']; ?>"><img src="/templates/img/delete.png" /></a></li>
                                        <?php } unset($__tplBlock[array_pop($__tpl_block_stack)]); endif; ?></ul>
				</article>
                            	<footer>
                                    <a href="javascript:toggle();"><?php echo $__tpl_vars__LANG_ADMIN_ARTICLE_ADD; ?></a>
				</footer>
			</article>
                        <article id="toggleText" style="display: none">
                                    <header>
                                            <h2><?php echo $__tpl_vars__LANG_ADMIN_ARTICLE_ADD; ?></h2>
                                    </header>
                                    <article>
                                        <form action="/admin/blog/addArticle/" method="post">
                                            <p><?php echo Talus_TPL_Filters::markdown($__tpl_vars__LANG_BLOG_USINGMD); ?></p>
                                            <input type="text" name="title" id="title" size="60"/>
                                            <textarea name="content" rows="16" cols="80" onFocus="this.value=''; this.onfocus=null;" >
                                            </textarea>
                                                   <select name="categorie" id="categorie">
                                                       <?php if ($tpl->block('menu_categories', null)) : $__tpl_c7accf2857775f3b432cb917398acd0720277db7 = &$tpl->block('menu_categories', null); $__tpl_block_stack[] = 'menu_categories'; foreach ($__tpl_c7accf2857775f3b432cb917398acd0720277db7 as &$__tplBlock['menu_categories']){ ?>
                                                       <option value="<?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_ID']; ?>"><?php echo $__tplBlock['menu_categories']['MENU_CATEGORY_NAME']; ?></option>
                                                       <?php } unset($__tplBlock[array_pop($__tpl_block_stack)]); endif; ?>
                                                   </select>
                                                <input type="radio" name="lang" value="fr" id="fr" /> <label for="fr">Français</label>
                                                <input type="radio" name="lang" value="en" id="en" /> <label for="en">English</label>
                                        <input type="submit" value="<?php echo $__tpl_vars__LANG_SEND; ?>" />    
                                        </form>
                                    </article>
                                    <footer>
                                    </footer>
                                
                        </article>
<?php $tpl->includeTpl('footer.html', false, Talus_TPL::INCLUDE_TPL); ?>