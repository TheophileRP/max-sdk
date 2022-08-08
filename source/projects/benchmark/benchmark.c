// see https://github.com/Cycling74/max-sdk/blob/main/source/basics/simplemax/simplemax.c
// Jeremy Bernstein

#include "ext.h"         // standard Max include, always required
#include "ext_obex.h"    // required for new style Max object
#include "../include/testInclude.h"

////////////////////////// object struct
typedef struct _benchmark {
	t_object ob;    // the object itself (must be first)
    t_object *patcher;
} t_benchmark;

///////////////////////// function prototypes
//// standard set
void* benchmark_new(t_symbol* s, long argc, t_atom* argv);
void  benchmark_free(t_benchmark* x);
void  benchmark_assist(t_benchmark* x, void* b, long m, long a, char* s);
void  benchmark_bang(t_benchmark* x);

//////////////////////// global class pointer variable
void* benchmark_class;


void ext_main(void* r) {
	t_class* c;

	c = class_new("benchmark", (method)benchmark_new, (method)benchmark_free, (long)sizeof(t_benchmark), 0L /* leave NULL!! */, A_GIMME, 0);

	/* you CAN'T call this from the patcher */
	class_addmethod(c, (method)benchmark_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)benchmark_bang,   "bang",           0);

	class_register(CLASS_BOX, c); /* CLASS_NOBOX */
	benchmark_class = c;

	post("I am the benchmark object");
    post("Benchmark : %s %s", __TIME__, __DATE__);
}

void benchmark_assist(t_benchmark* x, void* b, long m, long a, char* s) {
	if (m == ASSIST_INLET) {    // inlet
		sprintf(s, "I am inlet %ld", a);
	}
	else {    // outlet
		sprintf(s, "I am outlet %ld", a);
	}
}

void  benchmark_bang(t_benchmark* x){
    long deltaX = 110;
    t_object *prevObj, *obj;
    t_atom cord[4];
    
    prevObj = newobject_sprintf(x->patcher, "@maxclass newobj @text \"mapping toto\" @patching_position 10. 10.");
    
    for(int i = 1; i < 1000; i++){
        obj = newobject_sprintf(x->patcher, "@maxclass newobj @text \"mapping toto\" @patching_position %d 10.", (deltaX * i + 10));
        
        atom_setobj(cord, prevObj);
        atom_setlong(cord+1, 0);
        atom_setobj(cord+2, obj);
        atom_setlong(cord+3, 0);
        
        object_method_typed(x->patcher, gensym("connect"), 4, cord, NULL);
        prevObj = obj;
    }
//    newobject_sprintf(x->patcher, "@maxclass toggle @patching_position 10. 10.");
}


void benchmark_free(t_benchmark* x) {
	;
}

void* benchmark_new(t_symbol* s, long argc, t_atom* argv) {
	t_benchmark* x = NULL;
	long       i;
    
	if ((x = (t_benchmark*)object_alloc(benchmark_class))) {
        
        object_obex_lookup(x, gensym("#P"), &x->patcher);
        
		object_post((t_object*)x, "a new %s object was instantiated: %p", s->s_name, x);
		object_post((t_object*)x, "it has %ld arguments", argc);

		for (i = 0; i < argc; i++) {
			if ((argv + i)->a_type == A_LONG) {
				object_post((t_object*)x, "arg %ld: long (%ld)", i, atom_getlong(argv + i));
			}
			else if ((argv + i)->a_type == A_FLOAT) {
				object_post((t_object*)x, "arg %ld: float (%f)", i, atom_getfloat(argv + i));
			}
			else if ((argv + i)->a_type == A_SYM) {
				object_post((t_object*)x, "arg %ld: symbol (%s)", i, atom_getsym(argv + i)->s_name);
			}
			else {
				object_error((t_object*)x, "forbidden argument");
			}
		}
	}
    
    
    
    
	return (x);
}
