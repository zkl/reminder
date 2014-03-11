#ifndef CONFIG_H
#define CONFIG_H

struct config_key_t
{
	char key[128];
	char val[512];
};

typedef struct 
{
	int size;
	int last;
	int max;
	struct config_key_t * keys;
}config_t;

void config_init(config_t * config);
void config_free(config_t * config);

int config_load(config_t * config, const char * file);
int config_save(config_t * config, const char * file);

const char * config_get_val(config_t * config, const char * key);
void config_set_val(config_t * config, const char * key, const char * val);

#endif