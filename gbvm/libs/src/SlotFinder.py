# Define the keys and values
keys = [
    "(", ")", "{", "}", ";", ",", "+", "-", "*", "/",
    "%", "=", "<", ">", "<=", ">=", "==", "!=",
]

values = [
    "LPAREN", "RPAREN", "LBRACE", "RBRACE", "SEMICOLON", "COMMA",
    "PLUS", "MINUS", "STAR", "SLASH", "MOD", "EQUAL",
    "LT", "GT", "LE", "GE", "EQ", "NEQ",
]

table_size = 18

# Custom hash function
def string_hash_func(key, table_size):
    hash_value = 0
    for char in key:
        hash_value = hash_value * 37 + ord(char)
    return hash_value % table_size

# Entry structure (simplified for static allocation)
class Entry:
    def __init__(self, key=None, value=None):
        self.key = key
        self.value = value
        self.next = None

# Initialize entries array with None (fixed size)
entries = [None] * table_size

# Generate the entries and place them in the table
for key, value in zip(keys, values):
    slot = string_hash_func(key, table_size)
    
    # Handle collisions by linear probing (open addressing)
    while entries[slot] is not None:
        slot = (slot + 1) % table_size
    
    # Create a new entry and store it at the available slot
    entries[slot] = Entry(key, value)

# Output the static array for OpAndSepTokens
print("static Entry OpAndSepTokens[] = {")
for entry in entries:
    if entry:
        print(f'    {{"{entry.key}", (void*){entry.value}, NULL}},')
    else:
        print("    {NULL, NULL, NULL},")
print("};")
