import React, { useState } from "react";
import "./FlashcardViewer.css"; // Import the custom CSS file

const flashcards = 
[
	{
		title: "Math Basics",
		question: "What is 2 + 2?",
		answer: "The answer is 4.",
		difficulty: "Easy",
		category: "Math",
	},
	{
		title: "Science Facts",
		question: "What is the chemical symbol for water?",
		answer: "The chemical symbol for water is H2O.",
		difficulty: "Medium",
		category: "Science",
	},
];

const FlashcardViewer: React.FC = () => 
{
	const [currentCardIndex, setCurrentCardIndex] = useState(0);
	const [isFlipped, setIsFlipped] = useState(false);

	const currentCard = flashcards[currentCardIndex];

	const handleFlip = () => 
	{
		setIsFlipped(!isFlipped);
	};

	const handleNext = () => 
	{
		setIsFlipped(false);
		if (currentCardIndex < flashcards.length - 1) 
		{
			setCurrentCardIndex(currentCardIndex + 1);
		}
	};

	const handlePrevious = () => 
	{
		setIsFlipped(false);
		if (currentCardIndex > 0) 
		{
			setCurrentCardIndex(currentCardIndex - 1);
		}
	};

	return (
		<div className="min-h-screen bg-gradient-to-b from-black via-gray-900 to-black text-white flex flex-col items-center justify-center p-6">
			<h2 className="text-2xl font-bold mb-6">Flashcard Viewer</h2>

			<div className="relative w-full max-w-md h-48 cursor-pointer" onClick={handleFlip}>
				<div className={`card-inner ${isFlipped ? "is-flipped" : ""}`}>
					<div className="card-front absolute inset-0 bg-gray-800 text-white rounded-lg p-6 flex flex-col items-center justify-center">
						<h3 className="text-lg font-semibold mb-2">{currentCard.title}</h3>
						<p className="text-gray-300">{currentCard.question}</p>
						<p className="mt-4 text-sm text-gray-500">
							{currentCard.difficulty} - {currentCard.category}
						</p>
					</div>
					<div className="card-back absolute inset-0 bg-gray-900 text-white rounded-lg p-6 flex flex-col items-center justify-center">
						<h3 className="text-lg font-semibold mb-2">{currentCard.title}</h3>
						<p className="text-gray-300">{currentCard.answer}</p>
						<p className="mt-4 text-sm text-gray-500">
							{currentCard.difficulty} - {currentCard.category}
						</p>
					</div>
				</div>
			</div>
			
			<div className="mt-8 flex justify-between w-full max-w-md">
				<button className="px-4 py-2 bg-green-600 text-white rounded-full shadow-md hover:bg-green-500 disabled:opacity-50" onClick={handlePrevious} disabled={currentCardIndex === 0}>Previous</button>
				<button className="px-4 py-2 bg-green-600 text-white rounded-full shadow-md hover:bg-green-500 disabled:opacity-50" onClick={handleNext} disabled={currentCardIndex === flashcards.length - 1}>Next</button>
			</div>
		</div>
	);
};

export default FlashcardViewer;
