import os

# Create database file if it doesn't exist
open('portal/database/database.db', 'a').close()

# Get absolute path to database file
db_path = os.path.abspath("portal/database/database.db")

# Write file in "resources", holding absolute path to database file
f = open("resources/db_path.txt", "w")
f.write(db_path)
f.close()
