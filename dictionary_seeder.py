# A script that grabs 20000 words from this page:
# "https://en.wiktionary.org/wiki/Appendix:
# Frequency_dictionary_of_the_modern_Russian_language_(the_Russian_National_Corpus)"

# First step is download 20000 words and save them to database:
# 1- create_database()
# 2- scrape_words()
# 3- for word in words: insert_russian_word(word)
# -----------
# Second step is to use the yandex api.
# 1- translate_word(russian)
# 2- insert_english_word(english, russian)

import sqlite3 as sqlite
import requests
import threading
import json
# from multiprocessing import ThreadPool
from bs4 import BeautifulSoup as Soup

def get_connection():
	connection = sqlite.connect("dictionary.db")
	cursor     = connection.cursor()
	return (connection, cursor)

def create_table(connection, cursor):
	query = """create table if not exists words
				(id integer primary key autoincrement, russian text, english text)"""
	cursor.execute(query)
	connection.commit()
	

def close_connection(connection, cursor):
	cursor.close()
	connection.close()


# Seeds the database with russian words
def insert_russian_word(connection, cursor, russian):
	query = "insert into words(russian) values(?)"
	cursor.execute(query, (russian,))
	connection.commit()


# Inserts one english word, based on a russian word.
def insert_english_word(connection, cursor, english, russian):
	# Getting the russian word at first, to be used in a later insert statmenet.
	cursor.execute("select * from words where russian = ?", (russian,))
	russian_word = cursor.fetchone()

	# Checking if a russian word is found
	if russian_word:
		cursor.execute("update words set english = ? where russian = ?", (english, russian))
		connection.commit()

# Selects all available russian words from the database.
# To be used with translate_word, to fill the database with translations.
def select_russian_words(connection, cursor):
	cursor.execute("select * from words")
	words = cursor.fetchall()
	return words


# Translates a russian word from the Yandex API
def translate_word(russian):
	url  = "https://translate.yandex.net/api/v1.5/tr.json/translate"
	lang = "ru-en"
	key  = "trnsl.1.1.20170705T055512Z.437b273da55b7cc1.caf5daa0f6bb99bc383e2ba743ab2c699ba26973"
	request = requests.get(url, data={"key": key, "lang": lang, "text": russian })
	response = json.loads(request.text)
	# print(response.values())
	if "text" in response:
		return response["text"][0]
	else:
		return ""


# Scrapes 20000 words from wikipedia, and saves to database
def scrape_words():
	url = "https://en.wiktionary.org/wiki/Appendix:"
	url += "Frequency_dictionary_of_the_modern_Russian_language_(the_Russian_National_Corpus)"

	request = requests.get(url)
	soup    = Soup(request.content, "html.parser")

	# The parent div that contains all of the wanted words.
	parent_div = soup.find("div", {"class": "mw-parser-output"})
	link_tags  = parent_div.find_all("a")
	# Other links don't containt russian words.
	words      = link_tags[2:20003]
	return words

def main():
	connection, cursor = get_connection()
	create_table(connection, cursor)

	# scrapes and returns 20000 words.
	words = scrape_words()  
	for word in words:
		insert_russian_word(connection, cursor, word.get_text())

	# woman = translate_word("женщина")
	# boy   = translate_word("мальчик")

	# All russian words from the database
	russian_words = select_russian_words(connection, cursor)

	# for word in russian_words[17500:20003]:
	for word in russian_words:
		# Translates form yandex api, and returns either a word or an empty value
		russian_word = word[1]
		# print(russian_word)
		english_word = translate_word(russian_word)
		# print("%s = %s" % (russian_word, english_word))
		insert_english_word(connection, cursor, english_word, russian_word)


	close_connection(connection, cursor)

if __name__ == "__main__":
	main()