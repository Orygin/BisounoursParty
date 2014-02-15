<?php $tpl->includeTpl('header.html', false, Talus_TPL::INCLUDE_TPL); ?>
			<article>
				<header>
					<h2><?php echo $__tpl_vars__PAGE_TITLE; ?></h2>
				</header>
				<article>
					<?php echo Talus_TPL_Filters::markdown($__tpl_vars__LANG_ADMIN_WELCOME); ?>
				</article>
			</article>
<?php $tpl->includeTpl('footer.html', false, Talus_TPL::INCLUDE_TPL); ?>