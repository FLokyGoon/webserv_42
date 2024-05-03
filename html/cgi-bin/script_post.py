#!/usr/bin/python3

import sys
import json
import random

random_value1 = random.randint(1, 100)
random_value2 = random.choice(['Option 1', 'Option 2', 'Option 3'])

try:
    simulated_response_data = {
        "status": "success",
        "message": f"Randomly selected value: {random_value2}, Random number: {random_value1}"
    }
    post_result = f"Simulated POST Response: {json.dumps(simulated_response_data)}"
except Exception as e:
    post_result = f"Error simulating POST request: {e}"

response_body = f"""
<html>
<head></head>
<body>
    <h1>Form with Random Values</h1>
    <form action="/submit" method="post">
        <label for="randomNumber">Random Number:</label>
        <input type="text" id="randomNumber" name="randomNumber" value="{random_value1}" readonly><br>
        <label for="randomOption">Random Option:</label>
        <select id="randomOption" name="randomOption">
            <option selected>{random_value2}</option>
        </select><br>
        <input type="submit" value="Submit">
    </form>
    <p>{post_result}</p>
</body>
</html>
"""

content_length = len(response_body.encode('utf-8'))


print("HTTP/1.1 201 OK")
print("Content-Type: text/html")
print("Content-Length:", content_length)
print()


print(response_body)

sys.stdout.close()
