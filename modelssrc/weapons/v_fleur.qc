$cd "fleur\v_"
$scale 2
$modelname "Weapons/v_flower.mdl"
$body studio "flower_reference.smd"
$origin 13 8 10
$cdmaterials "models\Weapons\v_hand\"
$cdmaterials "models\Weapons\v_flower\"

$attachment 0 "bone01" 0 0 0

// TIGE

$jigglebone "bone_fleur03" {
	is_flexible {
		yaw_stiffness 50
		yaw_damping 15
		pitch_stiffness 50
		pitch_damping 15
		tip_mass 300
		length 20
		angle_constraint 20
	}
}
$jigglebone "bone_fleur04" {
	is_flexible {
		yaw_stiffness 50
		yaw_damping 15
		pitch_stiffness 50
		pitch_damping 15
		tip_mass 200
		length 20
		angle_constraint 20
	}
}
$jigglebone "bone_fleur05" {
	is_flexible {
		yaw_stiffness 50
		yaw_damping 15
		pitch_stiffness 50
		pitch_damping 15
		tip_mass 100
		length 20
		angle_constraint 20
	}
}

// FEUILLES
$jigglebone "bone_feuille01" {
	is_flexible {
		yaw_stiffness 70
		yaw_damping 5
		pitch_stiffness 100
		pitch_damping 8
		tip_mass 100
		length 20
		angle_constraint 30
	}
}
$jigglebone "bone_feuille02" {
	is_flexible {
		yaw_stiffness 70
		yaw_damping 5
		pitch_stiffness 100
		pitch_damping 8
		tip_mass 100
		length 20
		angle_constraint 30
	}
}

// PETALES
$jigglebone "bone_petale01" {
	is_flexible {
		yaw_stiffness 150
		yaw_damping 3
		pitch_stiffness 100
		pitch_damping 7
		tip_mass 50
		length 20
		angle_constraint 30
	}
}
$jigglebone "bone_petale02" {
	is_flexible {
		yaw_stiffness 150
		yaw_damping 3
		pitch_stiffness 100
		pitch_damping 7
		tip_mass 50
		length 20
		angle_constraint 30
	}
}
$jigglebone "bone_petale03" {
	is_flexible {
		yaw_stiffness 150
		yaw_damping 3
		pitch_stiffness 100
		pitch_damping 7
		tip_mass 50
		length 20
		angle_constraint 30
	}
}
$jigglebone "bone_petale04" {
	is_flexible {
		yaw_stiffness 150
		yaw_damping 3
		pitch_stiffness 100
		pitch_damping 7
		tip_mass 50
		length 20
		angle_constraint 30
	}
}
$jigglebone "bone_petale05" {
	is_flexible {
		yaw_stiffness 150
		yaw_damping 3
		pitch_stiffness 100
		pitch_damping 7
		tip_mass 50
		length 20
		angle_constraint 30
	}
}

$sequence idle "flower_idle" loop fps 28 activity ACT_VM_IDLE 1
$sequence draw "flower_draw" fps 90 snap activity ACT_VM_DRAW 1
$sequence swing01 "flower_swing01" fps 30 snap activity ACT_VM_PRIMARYATTACK 1
//$sequence swing02 "flower_swing02" fps 30 snap activity ACT_VM_PRIMARYATTACK 1
//$sequence swing03 "flower_swing03" fps 30 snap activity ACT_VM_PRIMARYATTACK 1
$sequence swing02 "flower_swing01" fps 30 snap activity ACT_VM_PRIMARYATTACK 1
$sequence swing03 "flower_swing01" fps 30 snap activity ACT_VM_PRIMARYATTACK 1