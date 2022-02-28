OLED_DRIVER_ENABLE = yes 	# Enables the use of OLED displays
ENCODER_ENABLE = yes		# Enables the use of one or more encoders
RGBLIGHT_ENABLE = yes		# Enable keyboard RGB underglow
LEADER_ENABLE = yes			# Enable the Leader Key feature
MOUSEKEY_ENABLE = no		# Enable Mouse Keys
TAP_DANCE_ENABLE = yes		# Enable Tap Dance
COMBO_ENABLE = yes			# Enable Combo
WPM_ENABLE = no

PIMORONI_TRACKBALL_ENABLE = no

ifeq ($(strip $(PIMORONI_TRACKBALL_ENABLE)), yes)
    SRC += pimoroni_trackball.c
    OPT_DEFS += -DPIMORONI_TRACKBALL_ENABLE
    POINTING_DEVICE_ENABLE := yes
endif
