import React, { useState } from "react";
import "./App.css";

import LandingPage from "./pages/LandingPage/LandingPage";
import CreateFlashcard from "./pages/CreateFlashcard/CreateFlashcard";
import FlashcardViewer from "./pages/FlashcardViewer/FlashcardViewer";
import FlashcardSetSelection from "./pages/FlashcardSetSelection/FlashcardSetSelection";

import { SignedIn, SignedOut, SignInButton, UserButton } from "@clerk/clerk-react";

enum Page 
{
	Landing,
	Create,
	Select,
	View,
}

interface FlashcardSetData 
{
    title: string;
    data: string;
}

function App() 
{
	const [currentPage, setCurrentPage] = useState<Page>(Page.Landing);
	const [selectedSetData, setSelectedSetData] = useState<FlashcardSetData | null>(null);

	const navigateToFlashcardSelection = () => 
	{
		setCurrentPage(Page.Select);
	};

	const handleSelectSet = (title: string, data: string) => 
	{
		setSelectedSetData({ title, data });

		setCurrentPage(Page.View);
	};

	return (
		<>
			<header className="absolute top-0 left-0 right-0 flex justify-between items-center p-4 z-10">
				<div className="text-lg font-bold text-white">Quizlytic</div>
				<div>
					<SignedIn>
						<UserButton/>
					</SignedIn>
				</div>
			</header>

			{currentPage === Page.Landing && (
				<LandingPage onClick={navigateToFlashcardSelection}/>
			)}

			{currentPage === Page.Select && (
				<FlashcardSetSelection 
					onSelectSet={handleSelectSet} 
				/>
			)}

			{currentPage === Page.View && selectedSetData !== null && (
				<FlashcardViewer title={selectedSetData.title} data={selectedSetData.data} onBack={navigateToFlashcardSelection} />
			)}
		</>
	);
}

export default App;
