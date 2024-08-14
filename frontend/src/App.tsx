import React, { useState } from "react";
import { BrowserRouter as Router, Route, Routes, useNavigate, Link } from "react-router-dom";
import "./App.css";

import LandingPage from "./pages/LandingPage/LandingPage";
import FlashcardViewer from "./pages/FlashcardViewer/FlashcardViewer";
import FlashcardSetSelection from "./pages/FlashcardSetSelection/FlashcardSetSelection";

import { SignedIn, UserButton } from "@clerk/clerk-react";

interface FlashcardSetData {
    title: string;
    data: string;
}

const App: React.FC = () => {
    const [selectedSetData, setSelectedSetData] = useState<FlashcardSetData | null>(null);
    const navigate = useNavigate();

    const handleSelectSet = (title: string, data: string) => {
        setSelectedSetData({ title, data });
        navigate(`/view`);
    };

    return (
        <>
            <header className="absolute top-0 left-0 right-0 flex justify-between items-center p-4 z-10">
                <div className="flex items-center space-x-4">
                    <div className="text-lg font-bold text-white">Quizlytic</div>
                    <Link to="/" className="text-white hover:text-gray-300">Home</Link>
                </div>
                <div className="ml-auto">
                    <SignedIn>
                        <UserButton />
                    </SignedIn>
                </div>
            </header>

            <Routes>
                <Route path="/" element={<LandingPage onClick={() => navigate("/select")} />} />
                <Route path="/select" element={<FlashcardSetSelection onSelectSet={handleSelectSet} />} />
                
                <Route path="/view" element={
                    <FlashcardViewer 
                        title={selectedSetData?.title || ''} 
                        data={selectedSetData?.data || ''} 
                        onBack={() => navigate('/select')}
                    />
                }/>
            </Routes>
        </>
    );
};

const AppWrapper = () => (
    <Router basename="/flashcards">
        <App />
    </Router>
);

export default AppWrapper;
