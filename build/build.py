def create_objs(environ, sources):
    return [environ.Object(s) for s in sources]