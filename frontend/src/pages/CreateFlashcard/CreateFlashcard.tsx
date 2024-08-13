import React, { useState } from "react";
import { z } from "zod";
import { useUser } from '@clerk/clerk-react';

const flashcardSchema = z.object({
	title: z.string().min(1, "Title is required"),
	description: z.string().min(1, "Description is required").max(50, "Your description is too long! Provide a brief description!"),
	difficulty: z.enum(["Easy", "Medium", "Hard"]),
});

interface Props 
{
	onClose: () => void;
}

const CreateFlashcardModal: React.FC<Props> = ({ onClose }) => 
{
	const { user } = useUser();
    const userId = user ? user.id : null;

	const [title, setTitle] = useState("");
	const [description, setDescription] = useState("");
	const [answer, setAnswer] = useState("");
	const [difficulty, setDifficulty] = useState(50);
	const [errors, setErrors] = useState<z.inferFlattenedErrors<typeof flashcardSchema> | null>(null);

	const getDifficultyLabel = () => 
	{
		if (difficulty < 34) return "Easy";
		if (difficulty < 67) return "Medium";
		return "Hard";
	};

	const handleSave = async () => 
	{
		const result = flashcardSchema.safeParse({title, description, difficulty: getDifficultyLabel(),});

		if (!result.success) 
			setErrors(result.error.flatten());
		else 
		{
			setErrors(null);

			try 
			{				
				const response = await fetch("http://localhost:8848/chat", 
				{
					method: "POST",
					headers: {
						"Accept": "application/json",
						"Content-Type": "application/json"
					},
					body: JSON.stringify({
						data: description + " with a difficulty of " + difficulty,
						clerkUserId: userId
					})
				});

				if (!response.ok)
				{
					throw new Error("Network response was not ok");
				}

				const data = await response.json();

				console.log(data);

				setAnswer(data.answer);
			} 
			catch (error) 
			{
				console.error("Error generating flashcard:", error);
			}
		}
	};

	return (
		<div className="fixed inset-0 bg-black bg-opacity-50 flex items-center justify-center z-50">
			<div className="bg-gray-800 rounded-lg shadow-lg p-6 w-full max-w-lg relative">
				<h2 className="text-xl font-bold mb-4 text-center text-white">Create Your Flashcard</h2>

				<button onClick={onClose} className="absolute top-2 right-2 text-gray-400 hover:text-white text-2xl"> &times; </button>

				<div className="mb-3">
					<label htmlFor="title" className="block text-sm font-medium text-gray-300">Flashcard Title</label>
					<input type="text" id="title" className="mt-1 block w-full rounded-md border-gray-700 bg-gray-900 text-white shadow-sm focus:border-green-500 focus:ring focus:ring-green-200 focus:ring-opacity-50 p-2" placeholder="Enter a title for your flashcard set" value={title} onChange={(e) => setTitle(e.target.value)}/>
					{errors?.fieldErrors?.title && <p className="text-red-500 text-sm">{errors.fieldErrors.title[0]}</p>}
				</div>

				<div className="mb-3">
					<label htmlFor="description" className="block text-sm font-medium text-gray-300">Description</label>
					<textarea id="description" rows={3} className="mt-1 block w-full rounded-md border-gray-700 bg-gray-900 text-white shadow-sm focus:border-green-500 focus:ring focus:ring-green-200 focus:ring-opacity-50 p-2" placeholder="Enter a description of the flashcards. For example: Make me 15 flashcards about common Algebra 1 topics." value={description} onChange={(e) => setDescription(e.target.value)}/>
					{errors?.fieldErrors?.description && <p className="text-red-500 text-sm">{errors.fieldErrors.description[0]}</p>}
				</div>

				<div className="mb-3">
					<label htmlFor="difficulty" className="block text-sm font-medium text-gray-300">Difficulty: {getDifficultyLabel()}</label>
					<input type="range" id="difficulty" className="w-full h-2 bg-gray-700 rounded-lg appearance-none cursor-pointer focus:outline-none focus:ring-2 focus:ring-green-500" min="0" max="100" value={difficulty} onChange={(e) => setDifficulty(Number(e.target.value))}/>
				</div>

				<div className="text-center">
					<button className="px-6 py-3 bg-green-600 text-white rounded-full font-bold shadow-md hover:bg-green-500" onClick={handleSave}> Generate Flashcard</button>
				</div>
			</div>
		</div>
	);
};

export default CreateFlashcardModal;
