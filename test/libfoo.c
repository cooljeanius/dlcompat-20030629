/* libfoo.c */

#ifdef HAVE_CONFIG_H
# include "../config.h"
#else
# define LIBFOO_C_NON_AUTOHEADER_BUILD 1
#endif /* HAVE_CONFIG_H */

/* not sure what was originally supposed to go in here, so just put a dummy
 * typedef so that this file will NOT be considered empty: */
typedef int libfoo_dummy_t;

/* EOF */
