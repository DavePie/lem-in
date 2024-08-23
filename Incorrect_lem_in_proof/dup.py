def find_duplicates(file_path):
    with open(file_path, 'r') as file:
        content = file.read()

    # Split the content by any whitespace (spaces, newlines, etc.)
    words = content.split()

    # Use a dictionary to count occurrences of each word
    word_count = {}
    for word in words:
        if word in word_count:
            word_count[word] += 1
        else:
            word_count[word] = 1

    # Find and print duplicates
    duplicates = {word: count for word, count in word_count.items() if count > 1}

    if duplicates:
        print("Duplicate words found:")
        for word, count in duplicates.items():
            print(f"'{word}' occurs {count} times.")
    else:
        print("No duplicates found.")

# Replace 'your_file.txt' with the path to your file
find_duplicates('paths.txt')