//
// language pack generator
//

#include "Language.h"
#include "includes.h"
#include "language_en.h"
#include "language_cn.h"
#include "language_ru.h"
#include "language_jp.h"
#include "language_de.h"
#include "language_am.h"
#include "language_cz.h"
#include "language_es.h"
#include "language_fr.h"
#include "language_pt.h"
#include "language_it.h"
#include "language_pl.h"
#include "language_sk.h"
#include "language_du.h"

const char *const en_pack[_LABEL_COUNT_] = {
#define X_WORD(NAME) EN_##NAME ,
#include "Language.inc"
#undef  X_WORD
        };

const char *const cn_pack[_LABEL_COUNT_] = {
#define X_WORD(NAME) CN_##NAME ,
#include "Language.inc"
#undef  X_WORD
        };

const char *const ru_pack[_LABEL_COUNT_] = {
#define X_WORD(NAME) RU_##NAME ,
#include "Language.inc"
#undef  X_WORD
        };

const char *const jp_pack[_LABEL_COUNT_] = {
#define X_WORD(NAME) JP_##NAME ,
#include "Language.inc"
#undef  X_WORD
        };

const char *const am_pack[_LABEL_COUNT_] = {
#define X_WORD(NAME) AM_##NAME ,
#include "Language.inc"
#undef  X_WORD
        };

const char *const de_pack[_LABEL_COUNT_] = {
#define X_WORD(NAME) DE_##NAME ,
#include "Language.inc"
#undef  X_WORD
        };

const char *const cz_pack[_LABEL_COUNT_] = {
#define X_WORD(NAME) CZ_##NAME ,
#include "Language.inc"
#undef  X_WORD
        };

const char *const es_pack[_LABEL_COUNT_] = {
#define X_WORD(NAME) ES_##NAME ,
#include "Language.inc"
#undef  X_WORD
        };

const char *const fr_pack[_LABEL_COUNT_] = {
#define X_WORD(NAME) FR_##NAME ,
#include "Language.inc"
#undef  X_WORD
        };

const char *const pt_pack[_LABEL_COUNT_] = {
#define X_WORD(NAME) PT_##NAME ,
#include "Language.inc"
#undef  X_WORD
        };

const char *const it_pack[_LABEL_COUNT_] = {
#define X_WORD(NAME) IT_##NAME ,
#include "Language.inc"
#undef  X_WORD
        };

const char *const pl_pack[_LABEL_COUNT_] = {
#define X_WORD(NAME) PL_##NAME ,
#include "Language.inc"
#undef  X_WORD
        };

const char *const sk_pack[_LABEL_COUNT_] = {
#define X_WORD(NAME) SK_##NAME ,
#include "Language.inc"
#undef  X_WORD
        };

const char *const du_pack[_LABEL_COUNT_] = {
#define X_WORD(NAME) DU_##NAME ,
#include "Language.inc"
#undef  X_WORD
        };

u8* language_text(const u8 label_index) {
    switch (infoSettings.language) {
    case LANG_ENGLISH:
        return (u8*) en_pack[label_index];
    case LANG_CHINESE:
        return (u8*) cn_pack[label_index];
    case LANG_RUSSIAN:
        return (u8*) ru_pack[label_index];
    case LANG_JAPANESE:
        return (u8*) jp_pack[label_index];
    case LANG_ARMENIAN:
        return (u8*) am_pack[label_index];
    case LANG_GERMAN:
        return (u8*) de_pack[label_index];
    case LANG_CZECH:
        return (u8*) cz_pack[label_index];
    case LANG_SPAIN:
        return (u8*) es_pack[label_index];
    case LANG_FRENCH:
        return (u8*) fr_pack[label_index];
    case LANG_PORTUGUESE:
        return (u8*) pt_pack[label_index];
    case LANG_ITALIAN:
        return (u8*) it_pack[label_index];
    case LANG_POLISH:
        return (u8*) pl_pack[label_index];
    case LANG_SLOVAK:
        return (u8*) sk_pack[label_index];
    case LANG_DUTCH:
        return (u8*) du_pack[label_index];
    default:
        return NULL;
    }
}
