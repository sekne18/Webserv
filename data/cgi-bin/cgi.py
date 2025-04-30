#!/usr/bin/env python3

import sys
import os

def main():
    try:
        content_length = int(os.environ.get("CONTENT_LENGTH", 0))
    except ValueError:
        content_length = 0

    post_data = sys.stdin.read(content_length)

    print("Content-Type: text/html")
    print()
    print("<html><body>")
    print("<h2>POST Data Received</h2>")
    print("<pre>{}</pre>".format(post_data))
    print("</body></html>")

if __name__ == "__main__":
    main()