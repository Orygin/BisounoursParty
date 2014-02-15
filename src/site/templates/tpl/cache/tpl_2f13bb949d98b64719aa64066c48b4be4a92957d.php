<?php $tpl->includeTpl('header.html', false, Talus_TPL::INCLUDE_TPL); ?>
			<article>
				<header>
					<h2><?php echo $__tpl_vars__PAGE_TITLE; ?></h2>
				</header>
				<article>
                                    <?php if ($__tpl_vars__ISCONNECTED == 1) : ?>
                                        <?php if ($__tpl_vars__USER_CLOUDLINKED == 0) : ?>
                                            <?php echo Talus_TPL_Filters::markdown($__tpl_vars__LANG_LINK_NOTLINKED); ?><br/>
                                            <a href="<?php echo $__tpl_vars__USER_CONNECTLINK; ?>"><img src="http://steamcommunity.com/public/images/signinthroughsteam/sits_small.png" /></a>
                                        <?php else : ?><p>
                                            <?php echo Talus_TPL_Filters::markdown($__tpl_vars__LANG_LINK_LINKED); ?><br/>
                                        <img    src="<?php echo $__tpl_vars__PLAYER_AVATAR; ?>" style="float:left;"/><div style="float:justified;"><br/><strong><?php echo $__tpl_vars__PLAYER_NAME; ?></strong></div><div style="float:right;"><a href="/link/unlink"><?php echo $__tpl_vars__LANG_LINK_UNLINK; ?></a></div><br/><br/><br/><br/>
                                        </p><?php endif; ?>
                                    <?php else : ?>
                                    <?php echo $__tpl_vars__LANG_MENU_NOTCONNECTED; ?>
                                    <?php endif; ?>
				</article>
			</article>
<?php $tpl->includeTpl('footer.html', false, Talus_TPL::INCLUDE_TPL); ?>