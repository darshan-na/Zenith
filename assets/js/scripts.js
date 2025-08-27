document.addEventListener('DOMContentLoaded', function() {
    const toggleButton = document.getElementById('theme-toggle');
    const html = document.documentElement;

    // Load saved theme from localStorage
    const savedTheme = localStorage.getItem('theme') || 'dark';
    html.setAttribute('data-theme', savedTheme);
    updateToggleIcon(savedTheme);
    updateHighlightJS(savedTheme);

    // Toggle theme on button click
    toggleButton.addEventListener('click', function(e) {
        e.preventDefault(); // Prevent default link behavior
        const currentTheme = html.getAttribute('data-theme');
        const newTheme = currentTheme === 'light' ? 'dark' : 'light';
        html.setAttribute('data-theme', newTheme);
        localStorage.setItem('theme', newTheme);
        updateToggleIcon(newTheme);
        updateHighlightJS(newTheme);
    });

    function updateToggleIcon(theme) {
        toggleButton.textContent = theme === 'light' ? '🌙' : '🔆'; // Cool moon for dark, bright sun for light
    }

    function updateHighlightJS(theme) {
        const darkThemeLink = document.getElementById('hljs-dark');
        const lightThemeLink = document.getElementById('hljs-light');
        if (theme === 'light') {
            darkThemeLink.disabled = true;
            lightThemeLink.disabled = false;
        } else {
            darkThemeLink.disabled = false;
            lightThemeLink.disabled = true;
        }
    }
});