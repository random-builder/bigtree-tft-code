
### format

properties
* file: bmp
* size: 95x95
* gimp export: no color space

### setup steps

add at the end

* boot.h/enum :          ICON_XXX
* boot.c/iconBmpName :   file-name

* Language.h/enum :      LABEL_XXX
* Language.c/en_pack :   EN_ICON_XXX
* language_en.h/define : EN_ICON_XXX
