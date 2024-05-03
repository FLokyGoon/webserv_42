#!/usr/bin/python3

"""
print("Content-type: text/plain\n")

while True:
    print("Looping infinitely")
    # Sleep for 1 second to prevent the script from consuming too much CPU
    import time
    time.sleep(1)

"""
import datetime
import sys

response_body = "<html><head></head><body><h1>" + datetime.datetime.strftime(datetime.datetime.now(), "%H:%M:%S") + "</h1></body></html>"

content_length = len(response_body)

print("HTTP/1.1 200 OK")
print("Content-Type: text/html")
print("Content-Length:", content_length)
print()
print(response_body)


sys.stdout.close()
