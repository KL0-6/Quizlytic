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

function App() 
{
	const [currentPage, setCurrentPage] = useState<Page>(Page.Landing);
	const [selectedSetId, setSelectedSetId] = useState<number | null>(null);

	const navigateToFlashcardSelection = () => 
	{
		setCurrentPage(Page.Select);
	};

	const handleSelectSet = (setId: number) => 
	{
		setSelectedSetId(setId);
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

			{currentPage === Page.View && selectedSetId !== null && (
				<FlashcardViewer/>
			)}
		</>
	);
}

export default App;
