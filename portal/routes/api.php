<?php

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Route;
use App\Models\Extension;
use App\Models\Generation;
use App\Models\Session;
use Jumper\GenerationState;
use Google\FlatBuffers\ByteBuffer;

/*
|--------------------------------------------------------------------------
| API Routes
|--------------------------------------------------------------------------
|
| Here is where you can register API routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| is assigned the "api" middleware group. Enjoy building your API!
|
*/

Route::middleware('auth:api')->get('/user', function (Request $request) {
    return $request->user();
});

Route::get('/sessions/{extensionId}', function ($extensionId) {
    return Extension::find($extensionId)->sessions()->get();
});

Route::get('/generations/{sessionId}', function ($sessionId) {
    return Session::find($sessionId)->generations()->select('id')->get();
});

Route::get('/extensions', function () {
    return Extension::all();
});

Route::get('/generation_scores/{generationId}', function ($generationId) {
    $stateBinary = Generation::find($generationId)->state;

    $byteBuffer = new ByteBuffer(strlen($stateBinary));
    for ($i = 0; $i < strlen($stateBinary); $i++) {
        $byteBuffer->put($i, $stateBinary[$i]);
    }

    $generationState = GenerationState::getRootAsGenerationState($byteBuffer);

    $scores = [];
    for ($i = 0; $i < $generationState->getPopulationLength(); $i++) {
        $scores[] = $generationState->getPopulation($i)->getScore();
    }

    rsort($scores);

    return $scores;
});
