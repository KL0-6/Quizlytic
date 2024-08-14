import React, { useState, useEffect } from "react";
import "./FlashcardViewer.css";
import { useNavigate } from "react-router-dom";

interface Flashcard
{
    question: string;
    answer: string;
}

interface Props 
{
	title: string;
    data: string;
    onBack: () => void;
}

const FlashcardViewer: React.FC<Props> = ({ title, data, onBack }) => 
{
    const [flashcards, setFlashcards] = useState<Flashcard[]>([]);
    const [currentCardIndex, setCurrentCardIndex] = useState(0);
    const [isFlipped, setIsFlipped] = useState(false);
	const navigate = useNavigate();

    useEffect(() => 
	{
		if (!title || !data) 
			return navigate('/select');
		
        try 
		{
            const parsedFlashcards: Flashcard[] = JSON.parse(data);
            setFlashcards(parsedFlashcards);
        } 
		catch (error) 
		{
            console.error("Invalid JSON string provided", error);
        }
    }, [data]);

    if (flashcards.length === 0) 
	{
        return (
            <div className="min-h-screen bg-gradient-to-b from-black via-gray-900 to-black text-white flex flex-col items-center justify-center p-6">
                <h2 className="text-2xl font-bold mb-6">No Flashcards Available</h2>
                <button className="mt-6 px-4 py-2 bg-blue-600 text-white rounded-full shadow-md hover:bg-blue-500" onClick={onBack}>Back to Set Selection</button>
            </div>
        );
    }

    const currentCard = flashcards[currentCardIndex];

    const handleFlip = () => 
	{
        setIsFlipped(!isFlipped);
    };

    const handleNext = () => 
	{
        setIsFlipped(false);
        if (currentCardIndex < flashcards.length - 1) 
            setCurrentCardIndex(currentCardIndex + 1);
    };

    const handlePrevious = () => 
	{
        setIsFlipped(false);

        if (currentCardIndex > 0)
            setCurrentCardIndex(currentCardIndex - 1);
    };

    return (
        <div className="min-h-screen bg-gradient-to-b from-black via-gray-900 to-black text-white flex flex-col items-center justify-center p-6">
            <h2 className="text-2xl font-bold mb-6">{title}</h2>

            <div className="relative w-full max-w-md h-48 cursor-pointer" onClick={handleFlip}>
                <div className={`card-inner ${isFlipped ? "is-flipped" : ""}`}>
                    <div className="card-front absolute inset-0 bg-gray-800 text-white rounded-lg p-6 flex flex-col items-center justify-center">
                        <p className="text-gray-300">{currentCard.question}</p>
                    </div>
                    <div className="card-back absolute inset-0 bg-gray-900 text-white rounded-lg p-6 flex flex-col items-center justify-center">
                        <p className="text-gray-300">{currentCard.answer}</p>
                    </div>
                </div>
            </div>

            <div className="mt-8 flex justify-between w-full max-w-md">
                <button className="px-4 py-2 bg-green-600 text-white rounded-full shadow-md hover:bg-green-500 disabled:opacity-50" onClick={handlePrevious} disabled={currentCardIndex === 0}>Previous</button>
                <button className="px-4 py-2 bg-green-600 text-white rounded-full shadow-md hover:bg-green-500 disabled:opacity-50" onClick={handleNext} disabled={currentCardIndex === flashcards.length - 1}>Next</button>
            </div>

            <button className="mt-6 px-4 py-2 bg-blue-600 text-white rounded-full shadow-md hover:bg-blue-500" onClick={onBack}>
                Back to Set Selection
            </button>
        </div>
    );
};

export default FlashcardViewer;
