<?php
    function IsAdmin($user)
    {
        if($user['group_id'] == 1 || $user['group_id'] == 5){
            return true;
        }
        else
            return false;
    }