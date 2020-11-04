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
    switch ($extensionName) {
        case "Jumper":
            $byteBuffer = new ByteBuffer(strlen($stateBinary));
            for ($i = 0; $i < strlen($stateBinary); $i++) {
                $byteBuffer->put($i, $stateBinary[$i]);
            }

            $generationState = GenerationState::getRootAsGenerationState($byteBuffer);

            for ($i = 0; $i < $generationState->getPopulationLength(); $i++) {
                $scores[] = $generationState->getPopulation($i)->getScore();
            }
            break;
    }

    rsort($scores);

    unset($generation->state);

    return ['scores' => $scores, 'generation' => $generation];
});
