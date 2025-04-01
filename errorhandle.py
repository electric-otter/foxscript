try:
    # Code that might raise an exception
    result = 10 / 0
except ZeroDivisionError as e:
    # Handle the exception
    print(f"Error: {e}")
else:
    print("No exception occurred.")
finally:
    print("This will always execute.")
