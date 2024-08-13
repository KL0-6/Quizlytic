/** @type {import('tailwindcss').Config} */
export default {
  content: [
    './src/**/*.{js,jsx,ts,tsx}', // Include your source files here
  ],
  theme: {
    extend: {
      colors: {
        'black': '#000000',
        'green-600': '#2f855a',
        'green-500': '#38a169',
        'green-700': '#276749',
        'green-800': '#22543d',
      },
      backgroundImage: theme => ({
        'gradient-to-r': 'linear-gradient(to right, #2f855a, #38a169)',
        'gradient-to-l': 'linear-gradient(to left, #2f855a, #38a169)',
      }),
      animation: {
        sparkle: 'sparkle 2s linear infinite',
      },
      keyframes: {
        sparkle: {
          '0%, 100%': { backgroundPosition: '0% 50%' },
          '50%': { backgroundPosition: '100% 50%' },
        },
      },
    },
  },
  plugins: [],
}
