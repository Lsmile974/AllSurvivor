
#ifndef ECS_EXPORT_H
#define ECS_EXPORT_H

#ifdef ECS_STATIC_DEFINE
#  define ECS_EXPORT
#  define ECS_NO_EXPORT
#else
#  ifndef ECS_EXPORT
#    ifdef ecs_EXPORTS
        /* We are building this library */
#      define ECS_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define ECS_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef ECS_NO_EXPORT
#    define ECS_NO_EXPORT 
#  endif
#endif

#ifndef ECS_DEPRECATED
#  define ECS_DEPRECATED __declspec(deprecated)
#endif

#ifndef ECS_DEPRECATED_EXPORT
#  define ECS_DEPRECATED_EXPORT ECS_EXPORT ECS_DEPRECATED
#endif

#ifndef ECS_DEPRECATED_NO_EXPORT
#  define ECS_DEPRECATED_NO_EXPORT ECS_NO_EXPORT ECS_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef ECS_NO_DEPRECATED
#    define ECS_NO_DEPRECATED
#  endif
#endif

#endif /* ECS_EXPORT_H */
