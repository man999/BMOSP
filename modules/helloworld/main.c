#include <system.h>

static const char name[] = "[APP]Привет мир!";
static const char message[] = "Привет из модуля!";
static const char logo[] = "\n\n"
                           "\t                                       ,:;;;;:,                                 \n"
                           "\t                                     :++******++:                               \n"
                           "\t                                    ;****+;;+****;                              \n"
                           "\t                                   :****;,  ,;****,                             \n"
                           "\t                                   :****:    :****:                             \n"
                           "\t                                   ,;****+;;+****;                              \n"
                           "\t                                    ,;+********+:                               \n"
                           "\t                                      ,:;****;:,                                \n"
                           "\t                                        :****,                                  \n"
                           "\t                                        :****,                                  \n"
                           "\t                                        :****,                                  \n"
                           "\t                                        :****,                                  \n"
                           "\t                                      ,:;****;:,,                               \n"
                           "\t                                      :*********+;:,                            \n"
                           "\t                                       :++;;+++****+;,                          \n"
                           "\t                                              ,:;****+:                         \n"
                           "\t                              ,,        ,,::,,   :+****:                        \n"
                           "\t                             ,++;;,   ,;+****+;,  ,+***+,                       \n"
                           "\t                             ;****:   ;********;   :****:                       \n"
                           "\t                             ;****,   ;********+   :****:                       \n"
                           "\t                             ,****;   ,;+*****;,  ,+***+,                       \n"
                           "\t                              ;****;,   ,::::,   ,+****;                        \n"
                           "\t                           ,:;+*****+;,,      ,,;+*****+;:,                     \n"
                           "\t                        ,:;+****++*****+;;;;;++****+++****+;:,                  \n"
                           "\t       ,,,::,,,      ,:;+****+;:, :;+***********++;, ,:;+****+;:,,:;;;;;;:,,    \n"
                           "\t    ,:+++****++;:,,:;+****+;:,      ,,:;;;;;;;::,       ,:;+****++*********+:,  \n"
                           "\t  ,;+****++++****++****+;:,                                ,:;+****+;;;;+****+, \n"
                           "\t ,+***+;:,,,,:;+****+;:,                                     :****;,     :+***+,\n"
                           "\t ;***+:        :+***;                                        ;***+,       :****:\n"
                           "\t,+***;          ;***+,                                       ;****:      ,;***+,\n"
                           "\t +***+,        ,+***+                                        ,+***+;:,,,:+****: \n"
                           "\t :+***+:,    ,:+***+:                                         ,;+***********+:  \n"
                           "\t  :+****++;;++****+,                                            ,:;+++++++;:,   \n"
                           "\t   ,:++********++:,                                                 ,,,,,       \n"
                           "\t      ,::;;;;::,                                                                "
                           "\n\n";
static const char logo_synapseos[] = "\n\n"
                                     "\t\t      ::::::::    :::   :::       ::::    :::           :::        ::::::::: "
                                     "      ::::::::       ::::::::::       ::::::::       ::::::::\n"
                                     "\t\t    :+:    :+:   :+:   :+:       :+:+:   :+:         :+: :+:      :+:    :+: "
                                     "    :+:    :+:      :+:             :+:    :+:     :+:    :+:\n"
                                     "\t\t   +:+           +:+ +:+        :+:+:+  +:+        +:+   +:+     +:+    +:+  "
                                     "   +:+             +:+             +:+    +:+     +:+\n"
                                     "\t\t  +#++:++#++     +#++:         +#+ +:+ +#+       +#++:++#++:    +#++:++#+    "
                                     "  +#++:++#++      +#++:++#        +#+    +:+     +#++:++#++\n"
                                     "\t\t        +#+      +#+          +#+  +#+#+#       +#+     +#+    +#+           "
                                     "        +#+      +#+             +#+    +#+            +#+\n"
                                     "\t\t#+#    #+#      #+#          #+#   #+#+#       #+#     #+#    #+#            "
                                     "#+#    #+#      #+#             #+#    #+#     #+#    #+#\n"
                                     "\t\t########       ###          ###    ####       ###     ###    ###             "
                                     "########       ##########       ########       ########\n"
                                     "\n\t\t\t\tМы вернулись!\n\n";

static int app_main( ) {
	log_printf("[%s]\n", message);
	return 2 + 2;
}

module_info_t __attribute__((section(".minit"))) init(env_t *env) {
	init_env(env);
	log_printf("[%s]\n", message);
	log_printf("%s\n", logo);
	log_printf("%s\n", logo_synapseos);
	return (module_info_t){ .name = (char *)&name,
		                    .message = (char *)&message,
		                    .type = 0,
		                    .data_size = 0,
		                    .data = (void *)&app_main,
		                    .err_code = 0,
		                    .module_id = 0,
		                    .irq = 0,
		                    .irq_handler = 0,
		                    .get_func = 0,
		                    .after_init = 0 };
}
