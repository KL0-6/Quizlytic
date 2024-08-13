import React from "react";
import "./LandingPage.css"

interface Props
{
	onClick: React.MouseEventHandler<HTMLButtonElement>;
}

const LandingPage: React.FC<Props> = ({ onClick }) =>
{
	return (
		<div className="relative min-h-screen bg-gradient-to-b from-black via-gray-900 to-black text-white overflow-hidden">
			<main className="flex flex-col justify-center items-center min-h-screen text-center px-4">
				<h1 className="text-4xl md:text-5xl font-extrabold leading-tight mb-4">
					Achieve more.
					<br/>
					<span className="relative inline-block animate-rotate-gradient bg-clip-text text-transparent">
						With Focused Flashcards.
					</span>
				</h1>
				
				<p className="max-w-md mb-6 text-base md:text-lg">
					Simply choose your subject, create your flashcards, and Quizlytic will help you master the material. Study smarter, not harder!
				</p>

				<div className="flex flex-col space-y-4 md:flex-row md:space-y-0 md:space-x-4 mb-8">
					<button onClick={onClick} className="w-full md:w-auto px-6 py-2 rounded-full text-sm md:text-lg font-bold shadow-lg text-white bg-green-600 hover:bg-green-500">
						Get Started
					</button>
				</div>
			</main>
            
			<footer className="absolute bottom-0 left-0 right-0 p-2 text-center bg-gradient-to-r from-gray-900 to-black text-xs md:text-sm">
				<p>
					Transform the way you study • Effortless Learning, Endless Possibilities • Your Study Assistant, Powered by Flashcards
				</p>
			</footer>
		</div>
	);
};

export default LandingPage;
