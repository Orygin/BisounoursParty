<?php
/**
 * Interface pour le compilateur de tpl.
 * (Facilite la Dependency Injection)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 * @package Talus' TPL
 * @author Baptiste "Talus" Clavié <clavie.b@gmail.com>
 * @copyright ©Talus, Talus' Works 2006+
 * @link http://www.talus-works.net Talus' Works
 * @link http://www.slideshare.net/fabpot/dependency-injection-with-php-53 Slideshare DI
 * @license http://www.gnu.org/licenses/lgpl.html LGNU Public License 3+
 * @version $Id$
 */

interface Talus_TPL_Compiler_Interface extends Talus_TPL_Dependency_Interface {
  /**
   * Setter pour les paramètres de compilation
   *
   * @param string $name Nom du paramètre
   * @param mixed $val Valeur du paramètre
   * @return mixed Valeur du paramètre $name
   */
  public function parameter($name, $val = null);

  /**
   * Compile la chaîne de caractère $str
   *
   * @param string $str Chaîne à compiler
   * @return string Code PHP résultant
   */
  public function compile($str);
}

/*
 * EOF
 */
