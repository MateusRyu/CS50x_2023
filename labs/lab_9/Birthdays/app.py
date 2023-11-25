import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # Get data from the form
        try:
            day = int(request.form.get("day"))
            month = int(request.form.get("month"))
            name = request.form.get("name")
        except ValueError:
            print("Invalid data types")
            return redirect("/")

        # Validate data
        if day < 1 or day > 31 or month < 1 or month > 12 or not name:
            return redirect("/")

        # Add the user's entry into the database
        db.execute("INSERT INTO birthdays (day, month, name) VALUES(?, ?, ?);", day, month, name)

        # Return to homepage
        return redirect("/")

    else:

        # Display the entries in the database on index.html
        people = db.execute("SELECT id, name, month, day FROM birthdays;")
        return render_template("index.html", people=people)

@app.route("/delete", methods=["POST"])
def delete():
    # Delete the user's entry into the database
    id = request.form.get("id")
    if id:
        db.execute("DELETE FROM birthdays WHERE id = ?;", id)
    return redirect("/")

@app.route("/edit")
def edit():
    id = request.args.get('id')
    if id:
        people = db.execute("SELECT id, name, month, day FROM birthdays WHERE id = ?;", id)
    return render_template("edit.html", person=people[0])

@app.route("/update", methods=["POST"])
def update():
    # Get data from the form
    name = request.form.get("name")
    try:
        day = int(request.form.get("day"))
        id = int(request.form.get("id"))
        month = int(request.form.get("month"))
    except ValueError:
        print("Invalid data types")
        return redirect("/")

    # Validate data
    if day < 1 or day > 31 or not id or month < 1 or month > 12 or not name:
        return redirect("/")

    # Update the user's entry into the database
    db.execute("UPDATE birthdays SET day = ?, month = ?, name= ? WHERE id = ?;", day, month, name, id)

    # Return to homepage
    return redirect("/")
