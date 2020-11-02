@extends('layout')

@section('title', 'Index')
@section('body')
<div id="app">
    {{-- Loading --}}
    <div v-if="!loaded" class="spinner-border" role="status">
        <span class="sr-only">Loading...</span>
    </div>

    {{-- Breadcrumbs --}}
    <nav v-if="loaded" aria-label="breadcrumb">
        <ol class="breadcrumb">
            <li class="breadcrumb-item" aria-current="page">
                <a href='#' v-on:click.prevent="getExtensions">Extensions</a>
            </li>
            <li v-if="selection.extension != null" class="breadcrumb-item" aria-current="page">
                <a href='#' v-on:click.prevent="getSessions(selection.extension)">Sessions of Extension
                    "@{{ selection.extension.name }}"</a>
            </li>
            <li v-if="selection.sessionId != null" class="breadcrumb-item" aria-current="page">
                <a href='#' v-on:click.prevent="getGenerations(selection.sessionId)">Generations of Session
                    @{{ selection.sessionId }}</a>
            </li>
            <li v-if="selection.generationId != null" class="breadcrumb-item" aria-current="page">
                <a href='#' v-on:click.prevent="getGenerations(selection.sessionId)">Generation
                    @{{ selection.generationId }}</a>
            </li>
        </ol>
    </nav>
    <h1 v-if="loaded">@{{ title }}</h1>

    {{-- Extensions --}}
    <table class="table mt-4" id='extensions' v-if='loaded && extensions.length > 0'>
        <thead>
            <tr>
                <th scope="col">#</th>
                <th scope="col">Name</th>
                <th scope="col">Actions</th>
            </tr>
        </thead>
        <tbody>
            <tr v-for="extension in extensions">
                <th scope="row">@{{ extension.id }}</th>
                <td>@{{ extension.name }}</td>
                <td>
                    <a v-on:click.prevent="getSessions(extension)" href='#'>Sessions</a>
                </td>
            </tr>
        </tbody>
    </table>

    {{-- Sessions --}}
    <table class="table mt-4" id='sessions' v-if='loaded && sessions.length > 0'>
        <thead>
            <tr>
                <th scope="col">#</th>
                <th scope="col">Actions</th>
            </tr>
        </thead>
        <tbody>
            <tr v-for="session in sessions">
                <th scope="row">@{{ session.id }}</th>
                <td>
                    <a v-on:click.prevent="getGenerations(session.id)" href='#'>Generations</a>
                </td>
            </tr>
        </tbody>
    </table>

    {{-- Generations --}}
    <table class="table mt-4" id='generations' v-if='loaded && generations.length > 0'>
        <thead>
            <tr>
                <th scope="col">#</th>
                <th scope="col">Actions</th>
            </tr>
        </thead>
        <tbody>
            <tr v-for="generation in generations">
                <th scope="row">@{{ generation.id }}</th>
                <td>
                    <a v-on:click.prevent="getPopulationScores(generation.id)" href='#'>Scores</a>
                </td>
            </tr>
        </tbody>
    </table>

    {{-- Population Scores --}}
    <canvas id='population-scores' v-show="scores.length > 0">

    </canvas>


    {{-- Item not found --}}
    <p class='mt-3' v-if="(sessions.length + extensions.length + generations.length + scores.length) == 0">
        No item found.
    </p>
</div>
@endsection