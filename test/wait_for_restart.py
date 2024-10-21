import time

def waiting_for_restart_before_embedding_tests(source, target, env):
    time.sleep(2)

Import("env")
env.AddPostAction("upload", waiting_for_restart_before_embedding_tests)