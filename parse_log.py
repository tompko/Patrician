import sys
import json
import webbrowser

# List similar commands so they end up in the same accordion
list_groups = [["usermove", "time", "otim"]]
groups = {}

for lg in list_groups:
	for g in lg:
		groups[g] = lg

white_piece_bitboards = [("White Pawn", "wp", "P"),
				("White Knight","wn", "N"),
				("White Bishop","wb", "B"),
				("White Rook","wr", "R"),
				("White Queen","wq", "Q"),
				("White King","wk", "K")]
black_piece_bitboards = [("Black Pawn","bp", "p"),
				("Black Knight","bn", "n"),
				("Black Bishop","bb", "b"),
				("Black Rook","br", "r"),
				("Black Queen","bq", "q"),
				("Black King","bk", "k")]

piece_list = ["p","P","n","N","b","B","r","R","q","Q","k","K"]

def bitboard_to_fen(bitboard, symbol):
	"""Generate a FEN from a given bitboard, using <symbol> to represent occupied squares"""
	bitboard = int(bitboard, 16)
	fen = ""
	for ra in range(7, -1, -1):
		for fi in range(8):
			if bitboard & (1 << (ra*8 + fi)):
				fen += symbol
			else:
				fen += "."
		fen += "/"

	for i in range(8, 0, -1):
		fen = fen.replace("."*i, str(i))

	return fen[:-1]

def move_to_fen(move):
	fen = ""

	for ra in range(7, -1, -1):
		for fi in range(8):
			square_index = ra*8 + fi
			if square_index == move["to"]:
				fen += "(.)"
			elif square_index == move["from"]:
				fen += "({0})".format(piece_list[move["piece"]])
			else:
				fen += "."
		fen += "/"

	for i in range(8,0,-1):
		fen = fen.replace("."*i, str(i))

	return fen[:-1]


def generate_log(path):
	"""Generate an html log from the debug .dat log file written out by Patrician"""
	with open("logs\\log.html", "w") as log_file:
		with open("logs\\header.html") as header:
			log_file.write(header.read())

		with open(path) as dat_file:
			line = dat_file.readline()
			while line:
				log_entry = json.loads(line.strip())
				log_type = log_entry["log_type"]
				title = log_entry["log_type"].title().replace("_", " ")
				if log_type == "input" or log_type == "xboard_input":
					# Write out this log entry and any following that share the same command
					# or command group
					entry_token = log_entry["data"].split()[0]
					title += " - {0}".format(entry_token)

					log_file.write('\t<h3><a href="#">{0}</a></h3>\n'.format(title))
					log_file.write('\t<div>\n')
					log_file.write('\t\t<p>{0}</p>\n'.format(log_entry["data"]))
					
					line = dat_file.readline()
					if not line: continue
					log_entry = json.loads(line.strip())
					
					while log_type == log_entry["log_type"] and \
						  (log_entry["data"].split()[0] == entry_token or\
						  	log_entry["data"].split()[0] in groups.get(entry_token, [])):
						log_file.write('\t\t<p>{0}</p>\n'.format(log_entry["data"]))
						line = dat_file.readline()
						log_entry = json.loads(line.strip())
					
					log_file.write('\t</div>\n')
				elif log_type == "xboard_unrecognised":
					# Write out the log entry as an error
					log_file.write('\t<h3 class="error"><a href="#">{0}</a></h3>\n'.format(title))
					log_file.write('\t<div>\n')
					log_file.write('\t\t{0}\n'.format(log_entry["data"]))
					log_file.write('\t</div>\n')
					line = dat_file.readline()
				elif log_type == "board":
					board = json.loads(log_entry["data"])
					log_file.write('\t<h3><a href="#">Board</a></h3>\n')
					log_file.write('\t<div>\n')
					log_file.write('\t\t<div>')
					for pb in white_piece_bitboards:
						log_file.write('\t\t<div style="float: left">\n')
						log_file.write('\t\t\t{0}<br />\n'.format(pb[0]))
						fen = bitboard_to_fen(board[pb[1]], pb[2])
						log_file.write('\t\t\t<c:chess>{0}</c:chess>\n'.format(fen))
						log_file.write('\t\t</div>\n')
					log_file.write("\t\t</div>")
					log_file.write('\t\t<div>')
					for pb in black_piece_bitboards:
						log_file.write('\t\t<div style="float: left">\n')
						log_file.write('\t\t\t{0}<br />\n'.format(pb[0]))
						fen = bitboard_to_fen(board[pb[1]], pb[2])
						log_file.write('\t\t\t<c:chess>{0}</c:chess>\n'.format(fen))
						log_file.write('\t\t</div>\n')
					log_file.write("\t\t</div>")

					occupied = bitboard_to_fen(board["oo"], "+")
					log_file.write('\t\t<div style="float: left">\n')
					log_file.write('\t\t\tOccupied<br />\n')
					log_file.write('\t\t\t<c:chess>{0}</c:chess>\n'.format(occupied))
					log_file.write('\t\t</div>\n')
					empty = bitboard_to_fen(board["ee"], "+")
					log_file.write('\t\t<div style="float: left">\n')
					log_file.write('\t\t\tEmpty<br />\n')
					log_file.write('\t\t\t<c:chess>{0}</c:chess>\n'.format(empty))
					log_file.write('\t\t</div>\n')
					enpassant = bitboard_to_fen(board["ep"], "+")
					log_file.write('\t\t<div style="float: left">\n')
					log_file.write('\t\t\tEn Passant<br />\n')
					log_file.write('\t\t\t<c:chess>{0}</c:chess>\n'.format(enpassant))
					log_file.write('\t\t</div>\n')
					log_file.write('\t\t<div>\n')
					log_file.write('\t\tSide to move: {0}\n'.format(board["si"]))
					log_file.write('\t\t</div>')
					log_file.write('\t</div>\n')
					line = dat_file.readline()
				elif log_type == "move":
					move = json.loads(log_entry["data"])
					log_file.write('\t<h3><a href="#">{0}</a></h3>\n'.format(title))
					log_file.write('\t<div>\n')
					move_board = move_to_fen(move)
					log_file.write('\t\t<c:chess>{0}</c:chess>\n'.format(move_board))
					log_file.write('\t\t{0}\n'.format(log_entry["data"]))
					log_file.write('\t</div>\n')
					line = dat_file.readline()
				else:
					raise ValueError("Unrecognised log entry: {0}".format(log_entry["log_type"]))


		with open("logs\\footer.html") as footer:
			log_file.write(footer.read())

if __name__ == "__main__":
	print "Generating log from:", sys.argv[1]
	generate_log(sys.argv[1])

	webbrowser.open("logs\\log.html")