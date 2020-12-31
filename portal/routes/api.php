<?php

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Route;
use App\Models\Extension;
use App\Models\Generation;
use App\Models\Session;
use Core\GenerationState as CoreGenerationState;
use Google\FlatBuffers\ByteBuffer;
use Illuminate\Support\Facades\Log;

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
    $extension = Extension::find($extensionId);
    $sessions = $extension->sessions()->orderBy('id', 'desc')->get();
    return ['sessions' => $sessions, 'extension' => $extension];
});

Route::get('/generations/{sessionId}', function ($sessionId) {
    $session = Session::find($sessionId);
    $generations = $session->generations()->select('id')->orderBy('id', 'desc')->get();

    $session->extension = $session->extension;

    return ['session' => $session, 'generations' => $generations];
});

Route::get('/extensions', function () {
    return Extension::all();
});

Route::get('/generation_scores/{generationId}', function ($generationId) {
    $generation = Generation::find($generationId);
    $stateBinary = $generation->state;
    $extensionName = $generation->session->extension->name;

    $scores = [];
    $byteBuffer = new ByteBuffer(strlen($stateBinary));
    for ($i = 0; $i < strlen($stateBinary); $i++) {
        $byteBuffer->put($i, $stateBinary[$i]);
    }

    $generationState = CoreGenerationState::getRootAsGenerationState($byteBuffer);

    for ($i = 0; $i < $generationState->getPopulationLength(); $i++) {
        $scores[] = $generationState->getPopulation($i)->getScore();
    }

    rsort($scores);

    unset($generation->state);

    return ['scores' => $scores, 'generation' => $generation];
});

Route::get('/session_scores/{sessionId}', function ($sessionId) {
    $session = Session::find($sessionId);
    $generations = $session->generations()->get();
    $session_scores = [];

    for ($i = 0; $i < count($generations); $i++) {
        $stateBinary = $generations[$i]->state;

        $generation_scores_average = 0;

        $byteBuffer = new ByteBuffer(strlen($stateBinary));
        for ($j = 0; $j < strlen($stateBinary); $j++) {
            $byteBuffer->put($j, $stateBinary[$j]);
        }

        $generationState = CoreGenerationState::getRootAsGenerationState($byteBuffer);

        for ($k = 0; $k < $generationState->getPopulationLength(); $k++) {
            $generation_scores_average += $generationState->getPopulation($k)->getScore();
        }

        $generation_scores_average /= $generationState->getPopulationLength();

        $session_scores[] = $generation_scores_average;
    }

    $session->extension = $session->extension;

    return ['scores' => $session_scores, 'session' => $session];
});
